/*----------------------------------------------------------------------------
   Component       : IPS - JSON
   Name            :
   Author          :
   Creation Date   : 07.MAY 2021
   Description     : json�ַ������������� 
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  JSONHELPER_H_
#define  JSONHELPER_H_
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <string>
#include <qstring.h>

#include "gespips.h"

class JsonHelper
{
public:
    //**************************************************
    //	brief   :�������յ�json�ַ���
    //	in      :
    //	out     :
    //	return  : true/false
    //**************************************************
    bool ParseFromString(const std::string &s);
    bool ParseFromString(const QString &s);

    //**************************************************
    //	brief   :����keyֵ��ȡvalue 
    //	in      :
    //	out     :
    //	return  :���ػ�ȡ���ַ��� 
    //**************************************************
    std::string GetValue(const std::string &sKey);
    QString GetValue(const QString &sKey);


private:
    QJsonDocument m_jsonDoc;
    QJsonParseError m_parseError;
    QJsonObject m_jsonObject;
};






#endif // JSONHELPER_H_ !   
