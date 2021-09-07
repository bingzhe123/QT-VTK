#include "JsonHelper.h"



/*-----------------------------------------------------------
***   Description:		
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
bool JsonHelper::ParseFromString(const std::string & s)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::ParseFromString");
    auto sTemp = QString::fromStdString(s);
    return ParseFromString(sTemp);
	

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "JsonHelper::ParseFromString");
}

/*-----------------------------------------------------------
***   Description:		
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
bool JsonHelper::ParseFromString(const QString & s)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::ParseFromString");
    m_jsonDoc = QJsonDocument::fromJson(s.toUtf8(), &m_parseError);
    if (m_parseError.error != QJsonParseError::NoError)
    {
        return false;
    }
    m_jsonObject = m_jsonDoc.object();
	
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::ParseFromString");
	return true;
}

/*-----------------------------------------------------------
***   Description:	
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
std::string JsonHelper::GetValue(const std::string & sKey)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::GetValue");
    auto sTemp = QString::fromStdString(sKey);

    return GetValue(sTemp).toStdString();

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::GetValue");
}

/*-----------------------------------------------------------
***   Description:
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
QString JsonHelper::GetValue(const QString & sKey)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::GetValue");
    if (m_jsonObject.contains(sKey))
    {
        return QString();
    }
    return m_jsonObject[sKey].toString();
	
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "JsonHelper::GetValue");
}
