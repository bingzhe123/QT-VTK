/*----------------------------------------------------------------------------
   Component       : IPS - MESSAGE
   Name            :
   Author          :
   Creation Date   : 06.MAY 2021
   Description     : 消息类型定义
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  MESSAGECOMANDIDDEF_H_
#define  MESSAGECOMANDIDDEF_H_

//读取到数据后发送到主窗口处理
#define CommunicationID  QEvent::User + 100
enum class MessageCode
{
    MSG_ERROR,
    MSG_INFO,                   
};


#endif // MESSAGECOMANDIDDEF_H_ !   

