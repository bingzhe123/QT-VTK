#include <QtWidgets/QApplication>
#include <QThread>
#include <QSplashScreen>
#include <QPixmap>
#include "atlstr.h"
#include "direct.h"
#include "vtkoutputwindow.h"
#include <vector>
#include <QSettings>

#include "gespips.h"
#include "IPSAPP.h"
#include "IPSLogin.h"
#include "IPSThreadSender.h"
#include "IPSThreadCoder.h"
#include "IPSThreadDecoder.h"
#include "IPSThreadReciver.h"
std::string DeviceID;
TCPClient instruct;
std::queue<int>commandQueue;
QMutex commandQueueMutex;
std::queue<std::string> commandContainer;
QMutex commandContainerMutex;
IPSThreadReciver *reciver;
int loginpicturechangetime;
GESPIPS *gespips;
QString ScreenShotPath, ScreenCapPath;//截屏存储路径，录屏存储路径
int warningTimes;		//异常次数
double avgCheckTime;	//平均判图时间
extern int packagesNum;	//判图次数
int ManualCheckTime;	//判图时长       1-99
int MachineMode;		//0为模拟器模式   1为测试模式
void ConfigINI();	
Permission *permission;
/*-----------------------------------------------------------
***   Description:     主程序入口
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
int main(int argc, char *argv[])
{


    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QApplication a(argc, argv);
	ConfigINI();
	warningTimes = 0;
	gespips=new GESPIPS(MachineMode);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Read ini Files");
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), DeviceID);

	LOG_INFO (GESP::DebugLogger::GetInstance(), "Image Process Station Start Up");
	/************************************/
	QPixmap splash_image(":/img/res/splashscreen.png");
	QScreen *screen= QGuiApplication::primaryScreen();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Screen Flush");
	QSplashScreen splash(splash_image.scaled(screen->geometry().width(), screen->geometry().height()));
	splash.showFullScreen();
	//splash.setWindowFlags(Qt::FramelessWindowHint);
	reciver = new IPSThreadReciver(0);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Socket Thread Init");

	IPSAPP *w=nullptr;
	IPSLogin loginwindow(w);
	
	QObject::connect(reciver, &IPSThreadReciver::LostConnection, &loginwindow, &IPSLogin::LostConnect);
	QObject::connect(reciver->decoder, &IPSThreadDecoder::LoginSignal, &loginwindow, &IPSLogin::LoginProcess);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Login Widget Init");
	loginwindow.showFullScreen();// FullScreen();
	splash.finish(&loginwindow);
	
    return a.exec();
}

void ConfigINI()
{
	
	QSettings iniFile("./IPS.ini", QSettings::IniFormat);
	iniFile.setIniCodec("utf-8");
	iniFile.childGroups();
	iniFile.beginGroup("MachineConfig");
	DeviceID=iniFile.value("DeviceID").toString().toStdString();
	MachineMode = iniFile.value("MachineMode").toInt();
	ManualCheckTime= iniFile.value("ManualCheckTime").toInt();
	loginpicturechangetime = iniFile.value("LoginPictureTime").toInt();
	iniFile.endGroup();
	iniFile.beginGroup("SaveConfig");
	ScreenShotPath = iniFile.value("ScreenShotPath").toString();
	ScreenCapPath = iniFile.value("ScreenCapPath").toString();
	iniFile.endGroup();
	if (ManualCheckTime > 99)ManualCheckTime = 99;
	if (ManualCheckTime < 1)ManualCheckTime = 1;
}

timeCount* translateTime(__int64 t)
{
	timeCount*c = new timeCount;
	c->day = t / PER_DAY;
	t = t % PER_DAY;
	c->hour = t / PER_HOUR;
	t = t % PER_HOUR;
	c->minute = t / PER_MINUTE;
	t = t % PER_MINUTE;
	c->second = t / PER_SECOND;
	t = t % PER_SECOND;
	c->ms = t;
	return c;
}


QString TimeCountTOTimeLog(timeCount* a)
{
	QString b;

	b += QString::number(a->day) + "day   ";
	if (a->hour > 9)
		b += QString::number(a->hour) + ":";
	else
		b += "0" + QString::number(a->hour) + ":";
	if (a->minute > 9)
		b += QString::number(a->minute) + ":";
	else
		b += "0" + QString::number(a->minute) + ":";
	if (a->second > 9)
		b += QString::number(a->second);
	else
		b += "0" + QString::number(a->second);
	return b;

}