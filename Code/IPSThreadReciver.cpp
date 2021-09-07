#include "IPSThreadReciver.h"
#include <QDebug>
#include <deque>
QMutex tcp_mutex(QMutex::Recursive);
QMutex decodeBuffer_mutex(QMutex::Recursive);
extern GESP::TCPClient instruct;
std::queue<char> decodeBuf;
bool connected = false;

void IPSThreadReciver::run()
{
	const int recvbuflen = 2048;

	coder->start();
	decoder->start();
	sender->start();
	while (!stopThread)
	{
		if (connected)
		{
			char buf[2048] = { 0 };

			tcp_mutex.lock();
			int timeout = instruct.GetReceiveTimeout();
			int recivelen = instruct.ReceiveBytes(buf, 2048);
			tcp_mutex.unlock();
			if (recivelen > 0)
			{
				ConvertBuf(buf, recivelen);
				//emit RunDecoder();
			}
			else if (recivelen == -2)
			{
				stop();
				connected = false;
				emit LostConnection();
			}
			msleep(timeout);
		}
	}
	QMutexLocker locker(&decodeBuffer_mutex);
	QueueClear(decodeBuf);
}
bool IPSThreadReciver::ConnectToScs()
{
	stopThread = false;
	QMutexLocker locker(&tcp_mutex);
	if (instruct.Connect("127.0.0.1", 9100))
	{
		instruct.SetReceiveTimeout(1000);     // unit: ms
		instruct.SetSendTimeout(1000);
		connected = true;
		return true;
	}
	connected = false;
	return false;
}
void IPSThreadReciver::stop()
{
	stopThread = true;
	coder->stop();
	decoder->stop();
	sender->stop();
}
IPSThreadReciver::IPSThreadReciver(QObject* parent)
{
	
	coder = new IPSThreadCoder(0);
	decoder = new IPSThreadDecoder(0);
	sender = new IPSThreadSender(0);
	stopThread = false;

}
void IPSThreadReciver::ConvertBuf(char *buf, int recivelen)
{
	QMutexLocker locker(&decodeBuffer_mutex);
	for (int i = 0; i < recivelen; i++)
	{
		decodeBuf.push(buf[i]);
	}
	std::string reciveinfo("Recive from SCS: ");
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),reciveinfo+ buf);
}
