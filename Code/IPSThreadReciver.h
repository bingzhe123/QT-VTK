#pragma once
#include <qthread.h>
#include <queue>
#include <QMutex>

#include "IPSThreadCoder.h"
#include "IPSThreadDecoder.h"
#include "IPSThreadSender.h"
#include "gespips.h"

class IPSThreadReciver :
	public QThread
{
	Q_OBJECT
signals:
	void LostConnection();
protected:
	volatile bool stopThread;
	void run();
private:
	

public:
	IPSThreadCoder *coder;
	IPSThreadDecoder *decoder;
	IPSThreadSender *sender;
	bool ConnectToScs();
	void stop();
	IPSThreadReciver(QObject* parent = 0);
	void ConvertBuf(char*,int);
}; 
template <typename T>
void QueueClear(std::queue<T>& q) {
	std::queue<T> empty;
	swap(empty, q);
}