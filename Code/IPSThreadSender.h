#pragma once
#include <qthread.h>
#include <QMutex>
#include <queue>
#include "gespips.h"

class IPSThreadSender :
	public QThread
{
	Q_OBJECT
protected:
	volatile bool stopThread;
	void run();
private:
public:
	void stop();
	IPSThreadSender(QObject* parent = 0);

	int Send(char*, const int);
};

