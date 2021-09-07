#pragma once
#include <qthread.h>

#include "gespips.h"

class IPSThreadHelper :
	public QThread
{
protected:
	volatile bool stop_thread;
	void run();
private:
public:
	void stop();
	IPSThreadHelper();
signals:
	void GetCTImageSignal(void);
	void LoginSignal(int);
	void GetDRImageSignal(void);
	void ChangePasswordSignal(int);
	void StartManualCheckSignal(int);
	void CheckProcessSignal(int);
	void ModelChangeSignal(int);
	void LookBackSignal(QString*);
	void AICheckLookBackSignal(QString*);	
};

