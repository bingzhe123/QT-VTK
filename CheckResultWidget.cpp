#include "CheckResultWidget.h"
extern int ManualCheckTime;
extern std::queue<int> commandQueue;
extern QMutex commandQueueMutex;
extern std::queue<std::string> commandContainer;
extern QMutex commandContainerMutex;
QString equipID;
QString sessionID;
int CheckTime;
extern QString userID;
extern bool recallMode ;
/*-----------------------------------------------------------
***   Description:     判图界面构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
CheckResultWidget::CheckResultWidget(std::shared_ptr<Container> spContainer, QWidget *parent)
	: BaseView(spContainer, parent)
{
	ui.setupUi(this);
	leftdown = new QButtonGroup(this);
	middown = new QButtonGroup(this);
	leftdown->addButton(ui.radio_auto);
	leftdown->addButton(ui.radio_manual);
	middown->addButton(ui.radio_horizontal);
	middown->addButton(ui.radio_vertical);
	ui.radio_horizontal->setChecked(true);
	ui.radio_manual->setChecked(true);
	ui.widget_5->setObjectName("framewidget");
	ui.widget_5->setStyleSheet("#framewidget{border:1px solid #642AB5;border-radius:4px;}");
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"CheckResultWidget::CheckResultWidget");
	time_control = new QTimer(this);
	connect(time_control, &QTimer::timeout, this, [=] 
	{
		if (time_left != 0) 
		{  
			time_left--; ShowTimeLeft(time_left);
		} else emit CheckTimeOut(); 
	});
	connect(this, &CheckResultWidget::CheckTimeOut, this, &CheckResultWidget::TimeOutReturn);
	connect(ui.pushButton_4, &QPushButton::clicked, this, &CheckResultWidget::CheckCompletedWarning);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &CheckResultWidget::CheckCompleted);
	connect(ui.radio_auto, &QRadioButton::clicked, this, &CheckResultWidget::SetAutoCheck);
	connect(ui.radio_manual, &QRadioButton::clicked, this, &CheckResultWidget::SetManualCheck);
	connect(ui.radio_horizontal, &QRadioButton::clicked, this, &CheckResultWidget::HoriCheck);

	connect(ui.radio_horizontal, &QRadioButton::clicked, this, &CheckResultWidget::OnHoriCheck);

	connect(ui.radio_vertical, &QRadioButton::clicked, this, &CheckResultWidget::VertiCheck);
	ui.pushButton_3->setDisabled(true);
	ui.pushButton_4->setDisabled(true);
	label_time_count = new QLabel(this);
	label_time_count->setStyleSheet("QLabel{border-image:url(:/img/res/timecount.png)}");
	time_left = 0;
}
void CheckResultWidget::OnHoriCheck()
{
	//emit ClearIamge();
}

/*-----------------------------------------------------------
***   Description:     判图界面析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
CheckResultWidget::~CheckResultWidget()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CheckResultWidget::~CheckResultWidget");
}

void CheckResultWidget::StartCheck(QString id)
{
	LOG_INFO(GESP::DebugLogger::GetInstance(),std::string("start check equip id:" + eid.toStdString()).c_str());
	sessionID = id;
	equipID = eid;
	std::time(&time_start_check);
	if (!recallMode)
	{
		time_control->start(1000);
		time_left = ManualCheckTime;
		ShowTimeLeft(time_left);
	}
	emit SetAICounts();
	ui.pushButton_3->setDisabled(false);
	ui.pushButton_4->setDisabled(false);
}

void CheckResultWidget::ShowTimeLeft(int a)
{
	ui.time1->setText(QString::number(a / 10));
	ui.time2->setText(QString::number(a % 10));
}

void CheckResultWidget::CheckCompleted()
{
	emit DisableToolsbar();
	if (!recallMode)
	{
		time_control->stop();
		CheckTime = ManualCheckTime - time_left;
	}
	ui.pushButton_3->setDisabled(true);
	ui.pushButton_4->setDisabled(true);
	GESP::JsonObject returndata;
	returndata.Set(ksCount, 0);
	returndata.Set(ksID, session_id.toStdString());
	returndata.Set(ksUserName, userID.toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(recallMode ? 14 : 6);
	commandContainer.push(returndata.Stringify());
	ui.time1->setText("0");
	ui.time2->setText("0");
	emit StopCheck();
	LOG_INFO(GESP::DebugLogger::GetInstance(),std::string("Manual Judegement Result Return equip id:" + eid.toStdString()).c_str());
}

void CheckResultWidget::CheckCompletedWarning()
{
	emit DisableToolsbar();
	if (!recallMode)
	{
		time_control->stop();
		CheckTime = ManualCheckTime - time_left;
	}
	ui.pushButton_3->setDisabled(true);
	ui.pushButton_4->setDisabled(true);
	GESP::JsonObject returndata;
	returndata.Set(ksCount, 1);
	returndata.Set(ksID, session_id.toStdString());
	returndata.Set(ksUserName, userID.toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(recallMode ? 14 : 6);
	commandContainer.push(returndata.Stringify());
	ui.time1->setText("0");
	ui.time2->setText("0");
	emit StopCheck();
	LOG_INFO(GESP::DebugLogger::GetInstance(),std::string("Manual Judegement Result Return equip id:" + eid.toStdString()).c_str());
}

void CheckResultWidget::TimeOutReturn()
{
	emit DisableToolsbar();
	if (!recallMode)
	{
		time_control->stop();
		CheckTime = ManualCheckTime - time_left;
	}
	ui.pushButton_3->setDisabled(true);
	ui.pushButton_4->setDisabled(true);
	GESP::JsonObject returndata;
	returndata.Set(ksCount, -1);
	returndata.Set(ksID, session_id.toStdString());
	returndata.Set(ksUserName, userID.toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(recallMode ? 14 : 6);
	commandContainer.push(returndata.Stringify());
	ui.time1->setText("0");
	ui.time2->setText("0");
	emit StopCheck();
	LOG_INFO(GESP::DebugLogger::GetInstance(),std::string("Manual Judegement Result Return equip id:" + eid.toStdString()).c_str());
}

void CheckResultWidget::SetVertiDisable()
{
	ui.radio_horizontal->setChecked(true);
}

void CheckResultWidget::paintEvent(QPaintEvent *)
{
	QRect rec = ui.time2->geometry();
	label_time_count->setGeometry(rec.x()-rec.width(), rec.y(), rec.width()*2, rec.height());
}

void CheckResultWidget::SetAutoCheck()
{
	emit AutoCheckSwitch(true);
}

void CheckResultWidget::SetManualCheck()
{
	emit AutoCheckSwitch(false);
}

void CheckResultWidget::StopRecallCheck()
{
	emit DisableToolsbar();
	
	ui.pushButton_3->setDisabled(true);
	ui.pushButton_4->setDisabled(true);
	ui.time1->setText("0");
	ui.time2->setText("0");
	LOG_INFO(GESP::DebugLogger::GetInstance(), std::string("Manual Judegement Result Return equip id:" + eid.toStdString()).c_str());
}
