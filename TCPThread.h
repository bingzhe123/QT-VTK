/*----------------------------------------------------------------------------
   Component       : IPS - TCP
   Name            :
   Author          :
   Creation Date   : 06.MAY 2021
   Description     : tcp通讯线程类,主线程发送消息到后端时候，先把消息放到线程中
					 接收到后端的消息时候会发送到主窗口
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  TCPTHREAD_H_
#define  TCPTHREAD_H_
#include <string>
#include <qobject.h>
#include <share.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/QHostAddress>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <qevent.h>
#include "MessageComandIDDef.h"
#include "gespips.h"


//通讯内容
struct CommandContext
{
    //Json格式的内容
    std::string sContent;
};

class BESendMsgEvent :public QEvent
{
public:
    BESendMsgEvent() :QEvent(QEvent::Type(CommunicationID)) {};
public:
    CommandContext m_context;
};

class TCPThread:public QObject  
{
    Q_OBJECT
public:
    TCPThread(QObject *pMainWindow);
    ~TCPThread();
    //**************************************************
    //	brief   : 线程执行函数
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Execute();

    //**************************************************
    //	brief   :向后端发送消息 
    //	in      :const CommandContext &context:消息的内容
    //	out     :
    //	return  : 
    //**************************************************
    void SendMsgToBE(const CommandContext &context);

    //**************************************************
    //	brief   :结束线程 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Shutdown();
private:
    //**************************************************
   //	brief   :连接到服务 
   //	in      :address, port,服务器的地址和端口号
   //	out     :
   //	return  :连接成功或失败 
   //**************************************************
    void ConnectToHost(const QHostAddress &address, quint16 port);

    //**************************************************
    //	brief   :向后端发送消息 
    //	in      :const CommandContext &context:消息的内容
    //	out     :
    //	return  : 
    //**************************************************
    void ExecuteSendToBe(const CommandContext &context);

    //**************************************************
    //	brief   :从线程中发送消息到m_pMainWindow中, 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void PostMsg(const CommandContext &context);


private slots: 

    //**************************************************
    //	brief   :接收服务器数据 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void ReadReady();
    
    //**************************************************
    //	brief   :socketError 
    //	in      :QAbstractSocket::SocketError：错误类型
    //	out     :
    //	return  : 
    //**************************************************
    void SocketError(QAbstractSocket::SocketError);


private:
    std::shared_ptr<QTcpSocket> m_tcpClient = nullptr;
    //指向主窗口
    QObject  *m_pMainWindow = nullptr;
    //地址
    QHostAddress m_address;
    //端口
    quint16 m_uiPort;

    qint64 m_checkSize;
    //缓存上一次或多次的未处理的数据
    //这个用来处理，重新粘包
    QByteArray m_buffer;

private:
    std::shared_ptr<std::thread> m_spThread;
    //新的消息放入队列中
    std::queue<CommandContext > m_tasks;
    // synchronization
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    bool m_stop = false;
};

#endif // TCPTHREAD_H_ !   

