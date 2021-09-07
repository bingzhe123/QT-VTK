#include "IPSLogin.h"
#include <QThread>
#include "Cell3D.h"

extern QMutex tcp_mutex;
extern TCPClient instruct;
extern std::queue<int>commandQueue;
extern QMutex commandQueueMutex;
extern std::queue<std::string> commandContainer;
extern QMutex commandContainerMutex;
extern IPSThreadReciver *reciver;
extern bool connected;
extern int MachineMode;
extern GESPIPS *gespips;
extern int logout;
extern bool loginSign;
extern double avgCheckTime;
int manage_permission = 1;
extern int warningTimes;
extern bool bInit;
extern int packagesNum;
extern std::string DeviceID;
QString userID;
extern int loginpicturechangetime;
IPSLogin::IPSLogin(IPSAPP * w, QWidget * parent)
	: QMainWindow(parent)
{
	std::time(&time);
	std::localtime(&time);
	ui.setupUi(this);
	login = false;
	connect(ui.login, &QPushButton::clicked, this, &IPSLogin::Login);
	mainwindow = w;
	setWindowFlags(Qt::FramelessWindowHint);
	setObjectName("minw");
	ui.login->setStyleSheet("QPushButton{color:#FFFFFF;background:#642AB5;border:0px solid #642AB5;border-radius:8px;font-size:28px;}"
		"QPushButton:hover{color:#FFFFFF;background:#9254DE;border:0px solid #642AB5;border-radius:8px;font-size:28px;}"
		"QPushButton:pressed{color:#FFFFFF;background:#391085;border:0px solid #642AB5;border-radius:8px;font-size:28px;}");
	ui.ide->setTextMargins(100, 0, 0, 0);
	ui.pwe->setTextMargins(100, 0, 0, 0);
	ui.ide->setStyleSheet( 
		"QLineEdit{border:1px solid #CDA8F0;border-radius:8px;background-color:#FFFFFF;color:#642AB5;font-size:24px;}"
		"QLineEdit::hover{border:1px solid #642AB5;border-radius:8px;background-color:#FDFBFF;color:#642AB5;}"
		"QLineEdit::focus{border:1px solid #642AB5;border-radius:8px;background-color:#FAF6FF;color:#642AB5;}"
	);
	ui.pwe->setStyleSheet(
		"QLineEdit{border:1px solid #CDA8F0;border-radius:8px;background-color:#FFFFFF;color:#642AB5;font-size:24px;}"
		"QLineEdit::hover{border:1px solid #642AB5;border-radius:8px;background-color:#FDFBFF;color:#642AB5;}"
		"QLineEdit::focus{border:1px solid #642AB5;border-radius:8px;background-color:#FAF6FF;color:#642AB5;}"
	);
	ui.tips->setStyleSheet(
		"QLabel{border:0px solid #FF4D4F;font-size:24px;color:#FF0000;}"
	);

	screen = QGuiApplication::primaryScreen();
	screensize = screen->availableGeometry();

	ratiox = screensize.width() / (double)1920;
	ratioy = screensize.height() / (double)1080;

	ui.pwe->setEchoMode(QLineEdit::Password);
	ReGeometry(ui.label_background);
	ReGeometry(ui.ide);
	ReGeometry(ui.pwe);
	ReGeometry(ui.label_left_circle);
	ReGeometry(ui.label_right_circle);
	ReGeometry(ui.label_pwid);
	ReGeometry(ui.label_userid);
	ReGeometry(ui.label_circle1);
	ReGeometry(ui.label_circle2);
	ReGeometry(ui.label_circle3);
	ReGeometry(ui.right);
	ReGeometry(ui.login);
	ReGeometry(ui.loginarea);
	ReGeometry(ui.label);
	ReGeometry(ui.line);
	ReGeometry(ui.line_2);
	ReGeometry(ui.seot);
	ReGeometry(ui.tips);
	
	ui.statusBar->hide();

	ui.mainToolBar->hide();
	loginOvertime = new QTimer(this);
	imagetime = new QTimer(this);
	hidetime = new QTimer(this);
	alph = 0;
	imageindex = 0;
	backgroundimage[0] = new QPixmap(":/img/res/login/background.png");
	*backgroundimage[0] = backgroundimage[0]->scaled(QSize(ui.label->geometry().width(), ui.label->geometry().height()));
	backgroundimage[1] = new QPixmap(":/img/res/login/background1.png");
	*backgroundimage[1] = backgroundimage[1]->scaled(QSize(ui.label->geometry().width(), ui.label->geometry().height()));
	backgroundimage[2] = new QPixmap(":/img/res/login/background2.png");
	*backgroundimage[2] = backgroundimage[2]->scaled(QSize(ui.label->geometry().width(), ui.label->geometry().height()));
	connect(loginOvertime, &QTimer::timeout, this, &IPSLogin::LoginTimeOut);
	connect(imagetime, &QTimer::timeout, this, &IPSLogin::ChangeBackGroundImage);
	connect(hidetime, &QTimer::timeout, this, &IPSLogin::ChangeHideImage);
	if (reciver->ConnectToScs()) reciver->start(); 
	else  ui.tips->setText(QString::fromLocal8Bit("无法连接到服务器"));
	if(loginpicturechangetime >0)
	imagetime->start(loginpicturechangetime *1000);
	ui.label_background->setPixmap(*backgroundimage[0]);
	

}


void IPSLogin::paintEvent(QEvent * event)
{
	

}

void IPSLogin::ChangeBackGroundImage()
{
	imageindex++;
	alph = 0;
	if (imageindex >= 2)
		imageindex = 0;
	switch (imageindex)
	{
	case 0:ui.label_background->setPixmap(*backgroundimage[1]);
		ui.label_circle1->setStyleSheet("border-image:url(:/img/res/login/circle.png);");
		ui.label_circle2->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);"); 
		ui.label_circle3->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);"); 
		break;
	case 1:ui.label_background->setPixmap(*backgroundimage[0]);
		ui.label_circle1->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);");
		ui.label_circle2->setStyleSheet("border-image:url(:/img/res/login/circle.png);");
		ui.label_circle3->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);"); 
		break;
	/*case 2: ui.label_background->setPixmap(*backgroundimage[1]);
		ui.label_circle1->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);");
		ui.label_circle2->setStyleSheet("border-image:url(:/img/res/login/emcircle.png);");
		ui.label_circle2->setStyleSheet("border-image:url(:/img/res/login/circle.png);"); 
		break;*/
	}
	hidetime->start(38);
}

void IPSLogin::ChangeHideImage()
{
	alph += 10;
	if (alph >= 255)
	{
		alph = 255;
		hidetime->stop();
	}
	QPixmap pixmap(*backgroundimage[imageindex]);

	QPixmap temp(pixmap);
	temp.fill(Qt::transparent);

	QPainter painter(&temp);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.drawPixmap(0, 0, pixmap);
	painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);

	painter.fillRect(temp.rect(), QColor(0, 0, 0, alph));
	painter.end();

	pixmap = temp;
	ui.label->setPixmap(pixmap);
	
}

IPSLogin::~IPSLogin()
{
	if (userID != "")
	{
		commandQueueMutex.lock();
		commandQueue.push(4);
		commandQueueMutex.unlock();
	}
	LOG_INFO(GESP::DebugLogger::GetInstance(), "User :"+userID.toStdString()+"   LogOut");
	time_t time_off;
	std::time(&time_off);
	time_t time_discount=time_off - time;
	LOG_INFO(GESP::DebugLogger::GetInstance(), "station ID:  " + DeviceID + "  Total Packages:   " + QString::number(packagesNum).toStdString() + "  warning Packages:   " + QString::number(warningTimes).toStdString() + "  Average Package Check Time:   " + QString::number(avgCheckTime).toStdString());
	avgCheckTime = 0;
	string final_time = "total run time:  " + TimeCountTOTimeLog(translateTime(time_discount)).toStdString();
	if(userID!=""&&connected)while (logout);
	reciver->stop();
	reciver->wait();
	instruct.Close();
	LOG_INFO(GESP::DebugLogger::GetInstance(),final_time);
	delete gespips;
}

void IPSLogin::LoginSuccess()
{
	LOG_INFO(GESP::DebugLogger::GetInstance(),"Login Success");
	login = true;
	bInit = false;
	userID = login_user_id;
	hide();
	ui.login->setDisabled(false);
	mainwindow = new IPSAPP;
	
	mainwindow->GetContainer()->GetWidget<Cell3D>(ks3DCellWidget)->StartInterctor();
	connect(mainwindow, &IPSAPP::ChangeUserSignal, this, &IPSLogin::ChangeUser);
}

void IPSLogin::LoginTimeOut()
{
	loginOvertime->stop();
	loginSign = false;
	QApplication::restoreOverrideCursor();
	ui.login->setDisabled(false);
	ui.tips->setText(QString::fromLocal8Bit("登陆超时"));
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Login Over Time");
}

void IPSLogin::ChangeUser()
{
	commandQueueMutex.lock();
	commandQueue.push(4);
	commandQueueMutex.unlock();
	LOG_INFO(GESP::DebugLogger::GetInstance(), "User :" + userID.toStdString() + "   LogOut");
	time_t time_off;
	std::time(&time_off);
	time_t time_discount = time_off - time;
	string final_time = "user login time:  " + TimeCountTOTimeLog(translateTime(time_discount)).toStdString();
	LOG_INFO(GESP::DebugLogger::GetInstance(),final_time);
	LOG_INFO(GESP::DebugLogger::GetInstance(),"change user");
	LOG_INFO(GESP::DebugLogger::GetInstance(),"user log out");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "station ID:  "+DeviceID + "  Total Packages:   "
		+ QString::number(packagesNum).toStdString() + "  warning Packages:   " + QString::number(warningTimes).toStdString()
		+ "  Average Package Check Time:   " + QString::number(avgCheckTime).toStdString());
	avgCheckTime = 0;
	
	mainwindow->close();

	show();
	delete mainwindow;
}

void IPSLogin::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Return)
	{
		if (ui.login->isEnabled())
			ui.login->click();
	}
}

void IPSLogin::ReGeometry(QWidget * widget)
{
	QRect geo = widget->geometry();
	widget->setGeometry(ratiox*geo.x(), ratioy*geo.y(), ratiox*geo.width(), ratioy*geo.height());
}

void IPSLogin::Login()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSLogin::Login");
	ui.tips->setText("");
	ui.login->setDisabled(true);
	
	if(connected)
	{
		LoginDataProcess();
	}
	else
	{
		if (!MachineMode)
		{
			if (!reciver->ConnectToScs())
			{
				ui.login->setDisabled(false);
				ui.tips->setText(QString::fromLocal8Bit("无法连接到服务器"));
			}
			else
			{
				reciver->start();
				LoginDataProcess();
			}
		}
		else
		{
			login_user_id = ui.ide->text();
			QString logstring = "login user:" + login_user_id;
			LOG_INFO(GESP::DebugLogger::GetInstance(), logstring.toStdString());
			LOG_DEBUG(GESP::DebugLogger::GetInstance(),"skip login");
			LoginSuccess();
		}
	}
}

void IPSLogin::LoginDataProcess()
{
	loginOvertime->start(5000);
	GESP::JsonObject logindata;
	loginSign = true;
	logindata.Set(ksName, ui.ide->text().toStdString());
	logindata.Set(ksPassword, ui.pwe->text().toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(1);
	commandContainer.push(logindata.Stringify());
	login_user_id = ui.ide->text();
	QString logstring = "login user:" + login_user_id;
	LOG_INFO(GESP::DebugLogger::GetInstance(), logstring.toStdString());
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void IPSLogin::LoginProcess(int value)
{
	loginSign = false;
	QApplication::restoreOverrideCursor();
	ui.login->setDisabled(false);
	loginOvertime->stop();
	if (!value)
	{
		LoginSuccess();
		LOG_INFO(GESP::DebugLogger::GetInstance(),"login success");
	}
	else
	{
		if (value == 1)
		{
			ui.tips->setText(QString::fromLocal8Bit("用户名错误请重新输入"));
			LOG_INFO(GESP::DebugLogger::GetInstance(),"Wrong user name");
		}
		else if(value == 3)
		{
			ui.tips->setText(QString::fromLocal8Bit("用户已登录"));
			LOG_INFO(GESP::DebugLogger::GetInstance(),"User has been login");
		}
		else
		{
			ui.tips->setText(QString::fromLocal8Bit("密码错误请重新输入"));
			LOG_INFO(GESP::DebugLogger::GetInstance(),"Wrong password");
		}
	}
}


void IPSLogin::LostConnect()
{
	if (login)
	{
		mainwindow->close();
		show();
		delete mainwindow;
	}
	ui.tips->setText(QString::fromLocal8Bit("与服务器断开连接"));
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Connection Lost");
}