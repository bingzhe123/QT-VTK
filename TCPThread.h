/*----------------------------------------------------------------------------
   Component       : IPS - TCP
   Name            :
   Author          :
   Creation Date   : 06.MAY 2021
   Description     : tcpͨѶ�߳���,���̷߳�����Ϣ�����ʱ���Ȱ���Ϣ�ŵ��߳���
					 ���յ���˵���Ϣʱ��ᷢ�͵�������
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


//ͨѶ����
struct CommandContext
{
    //Json��ʽ������
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
    //	brief   : �߳�ִ�к���
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Execute();

    //**************************************************
    //	brief   :���˷�����Ϣ 
    //	in      :const CommandContext &context:��Ϣ������
    //	out     :
    //	return  : 
    //**************************************************
    void SendMsgToBE(const CommandContext &context);

    //**************************************************
    //	brief   :�����߳� 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Shutdown();
private:
    //**************************************************
   //	brief   :���ӵ����� 
   //	in      :address, port,�������ĵ�ַ�Ͷ˿ں�
   //	out     :
   //	return  :���ӳɹ���ʧ�� 
   //**************************************************
    void ConnectToHost(const QHostAddress &address, quint16 port);

    //**************************************************
    //	brief   :���˷�����Ϣ 
    //	in      :const CommandContext &context:��Ϣ������
    //	out     :
    //	return  : 
    //**************************************************
    void ExecuteSendToBe(const CommandContext &context);

    //**************************************************
    //	brief   :���߳��з�����Ϣ��m_pMainWindow��, 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void PostMsg(const CommandContext &context);


private slots: 

    //**************************************************
    //	brief   :���շ��������� 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void ReadReady();
    
    //**************************************************
    //	brief   :socketError 
    //	in      :QAbstractSocket::SocketError����������
    //	out     :
    //	return  : 
    //**************************************************
    void SocketError(QAbstractSocket::SocketError);


private:
    std::shared_ptr<QTcpSocket> m_tcpClient = nullptr;
    //ָ��������
    QObject  *m_pMainWindow = nullptr;
    //��ַ
    QHostAddress m_address;
    //�˿�
    quint16 m_uiPort;

    qint64 m_checkSize;
    //������һ�λ��ε�δ���������
    //���������������ճ��
    QByteArray m_buffer;

private:
    std::shared_ptr<std::thread> m_spThread;
    //�µ���Ϣ���������
    std::queue<CommandContext > m_tasks;
    // synchronization
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    bool m_stop = false;
};

#endif // TCPTHREAD_H_ !   

