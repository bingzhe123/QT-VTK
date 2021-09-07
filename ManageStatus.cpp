#include "ManageStatus.h"
extern QString userID;
extern QMutex commandQueueMutex;
extern QMutex commandContainerMutex;

extern std::queue<int>commandQueue;
extern std::queue<std::string> commandContainer;

extern std::vector <std::string> reviewList;
extern std::vector <int> reviewLock;
extern QMutex reviewListMutex;

extern std::vector <std::string> userlist;

ManageStatus::ManageStatus(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setAutoFillBackground(true);

	QPalette p;
	p.setBrush(this->backgroundRole(), QBrush(QColor(51, 51, 51)));
	this->setPalette(p);

	setFixedWidth(200);
	begin_time = ui.begin_time;
	begin_time->setMaximumDate(QDate::currentDate());
	begin_time->setCalendarPopup(true);
	begin_time->setDateTime(QDateTime::currentDateTime().addDays(-3));

	end_time = ui.end_time;
	end_time->setMaximumDate(QDate::currentDate());
	end_time->setCalendarPopup(true);
	end_time->setDateTime(QDateTime::currentDateTime());

	operator_box = ui.operator_box;

	start_time = ui.start_time;
	begin_time->setButtonSymbols(QAbstractSpinBox::NoButtons);
	begin_time->setAlignment(Qt::AlignCenter);
	end_time->setButtonSymbols(QAbstractSpinBox::NoButtons);
	end_time->setAlignment(Qt::AlignCenter);
	stop_time = ui.stop_time;
	user_select = ui.user_select;
	
	list_area = ui.list_area;
	list_area->setFrameShape(QListWidget::NoFrame);
	
	list_area->setStyleSheet("QListWidget{color:white;border:none;background:#444444;}"
		"QListWidget::item{color:white;border:none;background:#444444;height:36px;}"
		"QListWidget::item:hover{background:#777777;}"
		"QListWidget::item::selected:active{background:#5d5d5d;}"
		"QListWidget::item:selected{background:#5d5d5d;}");
	list_area->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"       
		"background:transparent;border-radius:2px;margin:0px,0px,0px,0px;width:6px}"
		"QScrollBar::handle:vertical{background:#dbdbdb;border-radius:2px;width:6px;min-height:20px;}"    
		"QScrollBar::handle:vertical:hover{background:#d0d0d0;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"
		"QScrollBar::add-line:vertical{border:none;background:none;}"
		"QScrollBar::sub-line:vertical{border:none;background:none;}");
	operator_box->clear(); 

	connect(ui.pushButton, &QPushButton::clicked, this, &ManageStatus::GetReviewList);
	connect(list_area, &QListWidget::itemSelectionChanged, this, &ManageStatus::GetSessionID);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Init IPSManageStatus");

	ui.pushButton->setEnabled(false);
	//begin_time->setStyleSheet("QCalendarWidget::QTableView{height:0px;}");
}

void ManageStatus::GetReviewList()
{
	GESP::JsonObject reviewData;
	reviewData.Set(ksCommandIndex, ksSCSCommand[7]);
	reviewData.Set(ksUserName, userID.toStdString());
	std::string begintime= begin_time->dateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
	std::string endtime = end_time->dateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
	reviewData.Set(ksStartTime, begintime);
	reviewData.Set(ksEndTime, endtime);
	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(8);
	commandContainer.push(reviewData.Stringify());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
	LOG_INFO(GESP::DebugLogger::GetInstance(),"send SCS"+reviewData.Stringify());
}
void ManageStatus::GetSessionID()
{	
	QVariant sessionID=list_area->item(list_area->currentRow())->data(Qt::UserRole);
	GESP::JsonObject obj;
	obj.Set(ksCommandIndex,ksSCSCommand[8]);
	obj.Set(ksID, sessionID.toString().toStdString());
	
	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(9);
	commandContainer.push(obj.Stringify());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
	emit SelectPacage(sessionID.toString());
	emit ShowSessionID(sessionID.toString());
	LOG_INFO(GESP::DebugLogger::GetInstance(),"send SCS: " + obj.Stringify());
}

void ManageStatus::ClearOprator()
{
	operator_box->clear();
}

void ManageStatus::ClearItems()
{
	list_area->clear();
}

void ManageStatus::InsertReviewList()
{
	LOG_INFO(GESP::DebugLogger::GetInstance(),"recive session_ID list");
	list_area->clear();
	QVariant sessionID;
	reviewListMutex.lock();
	for (int i = 0; i < reviewList.size();i++)
	{
		sessionID = QString(reviewList[i].c_str());
		list_area->addItem(reviewList[i].c_str());
		list_area->item(i)->setData(Qt::UserRole,sessionID);
		list_area->item(i)->setToolTip(reviewList[i].c_str());
	}
	reviewListMutex.unlock();
}

void ManageStatus::InsertReviewUser()
{
	LOG_INFO(GESP::DebugLogger::GetInstance(), "recive User list");
	operator_box->clear();
	for (int i = 0; i < userlist.size(); i++)
	{
		operator_box->addItem(QString(userlist[i].c_str()),QString( userlist[i].c_str()));
	}
	operator_box->setCurrentIndex(0);
	ui.pushButton->setEnabled(true);
}

void ManageStatus::Clear()
{
	operator_box->clear();
	ui.pushButton->setEnabled(false);
}

