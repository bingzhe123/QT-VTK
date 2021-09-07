#include "TCPThread.h"
#include "qapplication.h"

/*-----------------------------------------------------------
***   Description:     TCP线程构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
TCPThread::TCPThread(QObject * pMainWindow)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::TCPThread");
    m_spThread = std::make_shared<std::thread>(&TCPThread::Execute, this);


	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::TCPThread");
}

/*-----------------------------------------------------------
***   Description:     TCP线程析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
TCPThread::~TCPThread()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::~TCPThread");
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
    m_condition.notify_all();
    m_spThread->join();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::~TCPThread");

}

/*-----------------------------------------------------------
***   Description:     TCP线程执行
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::Execute()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::Execute");
    for (;;)
    {
        ConnectToHost(m_address, m_uiPort);

        CommandContext context;

        {
            std::unique_lock<std::mutex> lock(this->m_queueMutex);
            this->m_condition.wait(lock,
                [this] { return this->m_stop || !this->m_tasks.empty(); });
            if (this->m_stop && this->m_tasks.empty())
                return;
            context = std::move(this->m_tasks.front());
            this->m_tasks.pop();
            ExecuteSendToBe(context);
        }
    }
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::~TCPThread");
}

/*-----------------------------------------------------------
***   Description:     TCP线程发送信息
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::SendMsgToBE(const CommandContext & context)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::SendMsgToBE");
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);

        // don't allow enqueueing after stopping the pool
        if (m_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        m_tasks.emplace(context);
    }
    m_condition.notify_one();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::SendMsgToBE");
}

/*-----------------------------------------------------------
***   Description:     TCP线程关闭
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::Shutdown()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::Shutdown");
    m_stop = true;
    m_condition.notify_all();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::Shutdown");
}

/*-----------------------------------------------------------
***   Description:     TCP线程连接到主机
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::ConnectToHost(const QHostAddress & address, quint16 port)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ConnectToHost");
    if (nullptr == m_tcpClient)
    {
        return;
    }
    m_tcpClient = std::make_shared<QTcpSocket>();
    m_tcpClient->connectToHost(address, port);
    //默认等待3秒
    if (m_tcpClient->waitForConnected())
    {
        qDebug() << "connect is ok";
    }
    else
    {
        qDebug() << "connect is fail";
        return;
    }
    connect(m_tcpClient.get(), SIGNAL(readyRead()), this, SLOT(ReadReady()));
    connect(m_tcpClient.get(), SIGNAL(error(QAbstractSocket::SocketError)), this,
        SLOT(SocketError(QAbstractSocket::SocketError)));
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ConnectToHost");
}

/*-----------------------------------------------------------
***   Description:     执行消息发送
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::ExecuteSendToBe(const CommandContext & context)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ExecuteSendToBe");
    QByteArray sendByte;
    QDataStream out(&sendByte, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_3);
    //设置大端模式，C++、JAVA中都是使用的大端，一般只有linux的嵌入式使用的小端
    out.setByteOrder(QDataStream::BigEndian);
    //占位符,这里必须要先这样占位，然后后续读算出整体长度后在插入
    out << (ushort)(0)<< context.sContent.c_str();
    //回到文件开头，插入真实的数值
    out.device()->seek(0);
    ushort len = (ushort)(sendByte.size());
    out << len;
    m_tcpClient->write(sendByte);
    //立即发送
    m_tcpClient->flush();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ExecuteSendToBe");
}

/*-----------------------------------------------------------
***   Description:     发送信息
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::PostMsg(const CommandContext &context)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::PostMsg");
    auto pEvent = new BESendMsgEvent();
    pEvent->m_context = context;
    qApp->postEvent(m_pMainWindow, pEvent);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::PostMsg");
}

/*-----------------------------------------------------------
***   Description:     TCP线程套接字错误
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::SocketError(QAbstractSocket::SocketError)
{

	//GESPIPS::ERRO(  "TCPThread::SocketError");
}

/*-----------------------------------------------------------
***   Description:     准备好读取
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::ReadReady()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ReadReady");
    //缓冲区没有数据，直接无视
    if (m_tcpClient->bytesAvailable() <= 0)
    {
        return;
    }
    //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
    QByteArray buffer;
    //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
    //所以使用while(m_tcpClient->bytesAvailable())意义不大，其实只执行一次。
    buffer = m_tcpClient->readAll();

    //上次缓存加上这次数据
    /**
        混包的三种情况，数据A、B，他们过来时有可能是A+B、B表示A包+B包中一部分数据，
        然后是B包剩下的数据，或者是A、A+B表示A包一部分数据，然后是A包剩下的数据与B包组合。
        这个时候，我们解析时肯定会残留下一部分数据，并且这部分数据对于下一包会有效，所以我们
        要和下一包组合起来。
    */
    m_buffer.append(buffer);


    ushort iMsgLen;
    int iTtotalLen = m_buffer.size();
    int iHeaderSize = sizeof(qint32);
    while (iTtotalLen)
    {
        //与QDataStream绑定，方便操作。
        QDataStream packet(m_buffer);
        packet.setByteOrder(QDataStream::BigEndian);

        //不够包头的数据直接就不处理。包头是qint32的数字，表示这次通讯发送的数据字节数
        if (iTtotalLen < iHeaderSize)
        {
            break;
        }
        //先读取包的大小
        packet >> iMsgLen;

        //如果不够长度等够了在来解析
        if (iTtotalLen < iMsgLen)
        {
            break;
        }
        auto spTempBuffer = std::shared_ptr<char>(new char[iMsgLen + 1], std::default_delete<char []>());
        char *pData = spTempBuffer.get();
        memset(pData, 0, iMsgLen + 1);
        packet.readRawData(pData, iMsgLen);
        CommandContext context;
        context.sContent = std::string(pData);
        PostMsg(context);
        //缓存多余的数据
        buffer = m_buffer.right(iTtotalLen - iMsgLen);
        //更新长度
        iTtotalLen = buffer.size();

        //更新多余数据
        m_buffer = buffer;
    }
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ReadReady");
}
