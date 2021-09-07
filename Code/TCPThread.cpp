#include "TCPThread.h"
#include "qapplication.h"

/*-----------------------------------------------------------
***   Description:     TCP�̹߳���
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
***   Description:     TCP�߳�����
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
***   Description:     TCP�߳�ִ��
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
***   Description:     TCP�̷߳�����Ϣ
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
***   Description:     TCP�̹߳ر�
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
***   Description:     TCP�߳����ӵ�����
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
    //Ĭ�ϵȴ�3��
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
***   Description:     ִ����Ϣ����
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::ExecuteSendToBe(const CommandContext & context)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ExecuteSendToBe");
    QByteArray sendByte;
    QDataStream out(&sendByte, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_3);
    //���ô��ģʽ��C++��JAVA�ж���ʹ�õĴ�ˣ�һ��ֻ��linux��Ƕ��ʽʹ�õ�С��
    out.setByteOrder(QDataStream::BigEndian);
    //ռλ��,�������Ҫ������ռλ��Ȼ�������������峤�Ⱥ��ڲ���
    out << (ushort)(0)<< context.sContent.c_str();
    //�ص��ļ���ͷ��������ʵ����ֵ
    out.device()->seek(0);
    ushort len = (ushort)(sendByte.size());
    out << len;
    m_tcpClient->write(sendByte);
    //��������
    m_tcpClient->flush();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ExecuteSendToBe");
}

/*-----------------------------------------------------------
***   Description:     ������Ϣ
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
***   Description:     TCP�߳��׽��ִ���
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::SocketError(QAbstractSocket::SocketError)
{

	//GESPIPS::ERRO(  "TCPThread::SocketError");
}

/*-----------------------------------------------------------
***   Description:     ׼���ö�ȡ
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TCPThread::ReadReady()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ReadReady");
    //������û�����ݣ�ֱ������
    if (m_tcpClient->bytesAvailable() <= 0)
    {
        return;
    }
    //��ʱ��ôӻ�����ȡ���������ݣ����ǲ�ȷ��ÿ��ȡ�������Ƕ��١�
    QByteArray buffer;
    //������ź�readyRead�����ģ�ʹ��readAllʱ��һ�ΰ���һ�ο��õ�����ȫ�ܶ�ȡ����
    //����ʹ��while(m_tcpClient->bytesAvailable())���岻����ʵִֻ��һ�Ρ�
    buffer = m_tcpClient->readAll();

    //�ϴλ�������������
    /**
        ������������������A��B�����ǹ���ʱ�п�����A+B��B��ʾA��+B����һ�������ݣ�
        Ȼ����B��ʣ�µ����ݣ�������A��A+B��ʾA��һ�������ݣ�Ȼ����A��ʣ�µ�������B����ϡ�
        ���ʱ�����ǽ���ʱ�϶��������һ�������ݣ������ⲿ�����ݶ�����һ������Ч����������
        Ҫ����һ�����������
    */
    m_buffer.append(buffer);


    ushort iMsgLen;
    int iTtotalLen = m_buffer.size();
    int iHeaderSize = sizeof(qint32);
    while (iTtotalLen)
    {
        //��QDataStream�󶨣����������
        QDataStream packet(m_buffer);
        packet.setByteOrder(QDataStream::BigEndian);

        //������ͷ������ֱ�ӾͲ�������ͷ��qint32�����֣���ʾ���ͨѶ���͵������ֽ���
        if (iTtotalLen < iHeaderSize)
        {
            break;
        }
        //�ȶ�ȡ���Ĵ�С
        packet >> iMsgLen;

        //����������ȵȹ�����������
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
        //������������
        buffer = m_buffer.right(iTtotalLen - iMsgLen);
        //���³���
        iTtotalLen = buffer.size();

        //���¶�������
        m_buffer = buffer;
    }
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TCPThread::ReadReady");
}
