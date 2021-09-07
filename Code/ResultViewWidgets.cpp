#include "ResultViewWidgets.h"
#include <QDebug>
extern std::vector <SuspiciousObject> suspiciousObjects;
extern std::vector <SuspiciousObject> AISuspiciousObjects;
extern std::vector <SuspiciousObject> suspReviewObjects;
extern QMutex suspiciousMutex;
extern QMutex AISuspiciousMutex;
extern QMutex suspReviewMutex;

/*-----------------------------------------------------------
***   Description:    判图子窗口构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
ResultViewWidgets::ResultViewWidgets(ContainerPtr spContainer, QWidget *parent,bool mode)
	: BaseView(spContainer, parent)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"ResultViewWidgets::ResultViewWidgets");
	ui.setupUi(this);
	reviewMode = mode;
	susp_number = 0;
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect mm = screen->availableGeometry();
	setFixedHeight(168);
	setFixedWidth(0.43*screen->geometry().width());
	frame = new QLabel(this);
	frame->setStyleSheet("QLabel{border-radius:4px;border:1px solid #D9D9D9;}");
#pragma region qss
	//ui.label_111->setStyleSheet("QLabel{border-image:url(:/img/res/pwar.png);};");

	setStyleSheet("QLabel{color:#333333;};");
#pragma endregion
	list_widget = new QListWidget(this);
	list_widget->setGeometry(geometry().x()+15, geometry().y()+15, geometry().width()-30, geometry().height()-30);
	list_widget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background:transparent;"
		"border-radius:2px;margin:0px,0px,0px,0px;width:6px}"
		"QScrollBar::handle:vertical{background:#dbdbdb;border-radius:2px;width:6px;min-height:20px;}" 
		"QScrollBar::handle:vertical:hover{background:#d0d0d0;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"
		"QScrollBar::add-line:vertical{ border: none;background: none;}"
		"QScrollBar::sub-line:vertical{border: none;background: none;}");
	list_widget->setStyleSheet(
		"QListWidget::Item{background:transparent;padding-top:5px;padding-bottom:5px;border-radius:23px;padding-left:15px;}"
		"QListWidget::Item:hover{background:#FAFAFA;padding-top:5px;padding-bottom:5px;border-radius:23px;padding-left:15px;}"
		"QListWidget::Item:selected{background:#F5F5F5;padding-top:5px;padding-bottom:5px;border-radius:23px;padding-left:15px;}"
		"QListWidget{background:transparent;outline:0px;}");
	list_widget->setFrameShape(QListWidget::NoFrame);


	total_number = new QLabel(this);
	current_selected = new QLabel(this);
	total_number->setText(QString::fromLocal8Bit("共") + QString::number(list_widget->count()) + QString::fromLocal8Bit("个"));

	connect(list_widget, &QListWidget::itemSelectionChanged, this, &ResultViewWidgets::CountChanged);
	total_number->setVisible(false);
	current_selected->setVisible(false);
	list_widget->raise();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "ResultViewWidgets::ResultViewWidgets");
	ResetHeight();
}

/*-----------------------------------------------------------
***   Description:    判图子窗口析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
ResultViewWidgets::~ResultViewWidgets()
{
	delete list_widget;
	delete total_number;
	delete current_selected;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "ResultViewWidgets::~ResultViewWidgets");
}

void ResultViewWidgets::CountChanged()
{

	int row = 0;
	while (row < (list_widget->count()))
	{
		if(list_widget->item(row)->isSelected())
		{
			current_selected->setText(QString::fromLocal8Bit("第")+QString::number(row + 1)+QString::fromLocal8Bit("个"));
			if (row < suspiciousObjects.size())
			{
				emit SelectSuspiciousObject(suspiciousObjects[row].uid);
			}
			return;
		}
		row++;
	}
}

void ResultViewWidgets::AddItemToSuspiciousContainer(double x, double y, double z, double l, double w, double h, QString uid)
{
	if (reviewMode)
	{
		suspiciousMutex.lock();
		susp_number++;
		SuspiciousObject obj(susp_number, x, z, w, y - x, l - z, h - w, 0, QString::fromLocal8Bit("刀具"));
		obj.uid = uid;
		suspiciousObjects.push_back(obj);

		suspiciousMutex.unlock();
		ChangeSuspiciousShow();
	}
	else
	{
		suspReviewMutex.lock();
		susp_number++;
		SuspiciousObject obj(susp_number, x, z, w, y - x, l - z, h - w, 0, QString::fromLocal8Bit("刀具"));
		obj.uid = uid;
		suspReviewObjects.push_back(obj);

		suspReviewMutex.unlock();
		ChangeSuspiciousShow();
	}
}

void ResultViewWidgets::SetAICount()
{
	AISuspiciousMutex.lock();
	susp_number = AISuspiciousObjects.size();
	AISuspiciousMutex.unlock();
}

void ResultViewWidgets::ChangeAIShow(bool show)
{
	if (show)
	{
		suspiciousMutex.lock();
		AISuspiciousMutex.lock();

		for (auto it = AISuspiciousObjects.begin(); it != AISuspiciousObjects.end(); it++)
		{
			for (auto its = suspiciousObjects.begin(); its != suspiciousObjects.end(); its++)
			{
				if ((*it).Code == (*its).Code)break;
			}
			suspiciousObjects.push_back(*it);
		}
		AISuspiciousMutex.unlock();
		suspiciousMutex.unlock();
	}
	else
	{
		suspiciousMutex.lock();
		AISuspiciousMutex.lock();

		for (auto it = AISuspiciousObjects.begin(); it != AISuspiciousObjects.end(); it++)
		{
			for (auto its = suspiciousObjects.begin(); its != suspiciousObjects.end(); its++)
			{
				if ((*it).Code == (*its).Code)
				{
					suspiciousObjects.erase(its);
					break;
				}
			}
		}
		AISuspiciousMutex.unlock();
		suspiciousMutex.unlock();
	}
	ChangeSuspiciousShow();
}

void ResultViewWidgets::ShowReview(QString sessionID,int lock)
{
	if (sessionID== currentReviewID)
	{
		suspReviewMutex.lock();


		for (auto it = suspReviewObjects.begin(); it != suspReviewObjects.end(); it++)
		{
			for (auto its = suspiciousObjects.begin(); its != suspiciousObjects.end(); its++)
			{
				if ((*it).Code == (*its).Code)break;
			}
			suspiciousObjects.push_back(*it);
		}

		suspReviewMutex.unlock();
	}
	emit ChangeLockStatus(lock);
}

void ResultViewWidgets::ClearSuspicious()
{
	suspiciousMutex.lock();
	int *p = new int[suspiciousObjects.size()];
	int i = 0;
	for (auto it = suspiciousObjects.begin(); it != suspiciousObjects.end(); it++, i++)
		p[i] = (*it).Code;
	suspiciousMutex.unlock();
	for (int k=0; k <i; k++)
	{
		RemoveItem(p[k]);
	}
	ResetHeight();
	emit ClearIamge();
}

void ResultViewWidgets::SetReviewID(QString a)
{
	currentReviewID = a;
}

void ResultViewWidgets::AddItemToWidgetList(QListWidget *list_widgets,int code,double z,double ρ,double m,QString type)
{
	ListWidget *list1 = new ListWidget(list_widgets,code,z,ρ,m);
	list1->item = new QListWidgetItem();

	list1->item->setSizeHint(QSize(list_widget->geometry().width()*0.9, 46));
	list_widgets->addItem(list1->item);
	list1->widget->setSizeIncrement(list_widget->geometry().width()*0.9, 46);
	list_widgets->setItemWidget(list1->item, list1->widget);
	list1->item->setData(Qt::UserRole, code);
	connect(list1->reject_button, &QPushButton::clicked, this, [=] {
		{ RemoveItem(((QPushButton*)sender())->property("code").toInt()); }});
	ResetHeight();
}

void ResultViewWidgets::RemoveItem(int code)
{
	int row = 0;
	int line;
	int count = list_widget->count();
	if (!reviewMode)
	{
		suspReviewMutex.lock();
		for (auto it = suspReviewObjects.begin(); it != suspReviewObjects.end(); it++)
		{
			if ((*it).Code == code)
			{
				suspReviewObjects.erase(it);
				break;
			}
		}
		suspReviewMutex.unlock();

		while (row < (list_widget->count()))
		{
			line = list_widget->item(row)->data(Qt::UserRole).toInt();
			if (code == line)
			{
				list_widget->takeItem(row);
				total_number->setText(QString::fromLocal8Bit("共") + QString::number(list_widget->count()) + QString::fromLocal8Bit("个"));
				if (row == current_selected->text().toInt() - 1)
					current_selected->setText("");
				break;
			}
			row++;
		}
		ResetHeight();
	}
	else
	{
		AISuspiciousMutex.lock();
		for (auto it = AISuspiciousObjects.begin(); it != AISuspiciousObjects.end(); it++)
		{
			if ((*it).Code == code)
			{
				AISuspiciousObjects.erase(it);
				break;
			}
		}
		AISuspiciousMutex.unlock();

		suspiciousMutex.lock();
		for (auto it = suspiciousObjects.begin(); it != suspiciousObjects.end(); it++)
		{
			if ((*it).Code == code)
			{
				emit RemoveSuspiciousObject(it->uid);
				suspiciousObjects.erase(it);
				break;
			}
		}
		suspiciousMutex.unlock();

		while (row < (list_widget->count()))
		{
			line = list_widget->item(row)->data(Qt::UserRole).toInt();
			if (code == line)
			{
				list_widget->takeItem(row);
				total_number->setText(QString::fromLocal8Bit("共") + QString::number(list_widget->count()) + QString::fromLocal8Bit("个"));
				if (row == current_selected->text().toInt() - 1)
					current_selected->setText("");
				break;
			}
			row++;
		}
		ResetHeight();
	}
}

void ResultViewWidgets::ChangeSuspiciousShow()
{
	if (!reviewMode)
	{
		suspReviewMutex.lock();
		counts = suspReviewObjects.size();
		int count = list_widget->count();
		for (int i = 0; i < count; i++)
		{
			list_widget->takeItem(0);
		}

		for (int i = 0; i < counts; i++)
		{
			AddItemToWidgetList(list_widget, suspReviewObjects[i].Code);
		}
		suspReviewMutex.unlock();
	}
	else
	{
		suspiciousMutex.lock();
		counts = suspiciousObjects.size();
		int count = list_widget->count();
		for (int i = 0; i < count; i++)
		{
			list_widget->takeItem(0);
		}

		for (int i = 0; i < counts; i++)
		{
			AddItemToWidgetList(list_widget, suspiciousObjects[i].Code);
		}
		suspiciousMutex.unlock();
	}
}

void ResultViewWidgets::resizeEvent(QResizeEvent * )
{
	frame->setGeometry(0, 0,geometry().width(),geometry().height());
	list_widget->raise();
}

void ResultViewWidgets::ResetHeight()
{
	int num;
	if (!reviewMode)
	{
		suspReviewMutex.lock();
		num = suspReviewObjects.size();
		suspReviewMutex.unlock();
	}
	else
	{
		suspiciousMutex.lock();
		num = suspiciousObjects.size();
		suspiciousMutex.unlock();
	}
	if (num >= 3)
		setFixedHeight(168);
	else if (num == 0)
		setFixedHeight(0);
	else if (num == 1)
		setFixedHeight(76);
	else setFixedHeight(122);
}
