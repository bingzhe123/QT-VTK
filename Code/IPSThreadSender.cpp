#include "IPSThreadSender.h"

extern GESP::TCPClient instruct;
extern QMutex tcp_mutex;

QMutex encodeBufferMutex(QMutex::Recursive);
std::vector<char> sendBuffer;

extern bool connected;
void IPSThreadSender::run()
{
	while (!stopThread)
	{
		if (connected)
		{
			encodeBufferMutex.lock();
			if (!sendBuffer.empty())
			{
				Send(sendBuffer.data(), sendBuffer.size());
				sendBuffer.clear();
			}
			encodeBufferMutex.unlock();
			msleep(50);
		}
	}
}

void IPSThreadSender::stop()
{
	stopThread = true;
}

IPSThreadSender::IPSThreadSender(QObject* parent)
{
	stopThread = false;
}

int IPSThreadSender::Send(char *buffer, const int length)
{
	std::string DataSent =  buffer;
	std::string DataDebug = "Send SCS :" + DataSent;
	QMutexLocker locker(&tcp_mutex);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  DataDebug);
	return instruct.SendBytes(buffer, length);
}

