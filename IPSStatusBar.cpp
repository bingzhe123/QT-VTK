#include "IPSStatusBar.h"
#include "IPSAPP.h"

extern std::queue<int> commandQueue;
extern QMutex commandQueueMutex;
extern int manage_permission;
int packagesNum;
bool bInit = false;
/*-----------------------------------------------------------
***   Description:     IPS状态栏构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
IPSStatusBar::IPSStatusBar(std::shared_ptr< Container> spContainer,
    QWidget *parent)
    : BaseView(spContainer, parent)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSStatusBar::IPSStatusBar");
    ui.setupUi(this);
	deviceInfo = ui.pushButton;
	packagesNum = 0;
	change_user_button = ui.pushButton_changeuser;
    this->setFixedWidth(175);
	change_user_button->setText(QString::fromLocal8Bit("切换判图员"));
	ui.pushButton->setText(QString::fromLocal8Bit("设备信息"));
	ui.pushButton_ipsmanage->setText(QString::fromLocal8Bit("图像管理"));

	ui.pushButton_modeldrct->setText(QString::fromLocal8Bit("DR+CT模式"));
	
	//setWindowFlags(Qt::FramelessWindowHint);
	//ui.pushButton_ipsmanage->setProperty("choosen", true);
	ui.pushButton_modelct->setProperty("choosen", true);
	ui.pushButton_modeldr->setProperty("choosen", false);
	ui.pushButton_modeldrct->setProperty("choosen", false);
    setAutoFillBackground(true);
    //QPalette pal(this->palette();
    //QColor bk(70, 70,255);
    //pal.setColor(QPalette::Background,bk); //设置背景黑色
    //setPalette(pal);
    QPalette myPalette;
    myPalette.setBrush(backgroundRole(), QColor(10, 25, 69));
    this->setPalette(myPalette);

    setAttribute(Qt::WA_StyledBackground);
	setStyleSheet("QWidget{background:#0;}");


	connect(ui.pushButton_modelct, &QPushButton::clicked, this, &IPSStatusBar::OnCtButtonClicked);
	connect(ui.pushButton_modeldr, &QPushButton::clicked, this, &IPSStatusBar::OnDrButtonClicked);
	connect(ui.pushButton_modeldrct, &QPushButton::clicked, this, &IPSStatusBar::OnCtButtonClicked);

	//connect(//ui.pushButton_ipsmanage, &QPushButton::clicked, this, &IPSStatusBar::on_ipsmanagebutton_clicked);
	//connect(//ui.pushButton_scsmanage, &QPushButton::clicked, this, &IPSStatusBar::on_scsmanagebutton_clicked);
	setTipStatus(tipOn);
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &IPSStatusBar::showtime);
	timer->start(1000);
	//setGeometry(0, 0, 140, 900);
	ui.widget_t1->setObjectName("wt1");
	ui.widget_t1->setStyleSheet("#wt1{ background:rgba(255,255,255,0.10);border-radius:10px;}");
	ui.widget_t2->setObjectName("wt2");
	ui.widget_t2->setStyleSheet("#wt2{ background:rgba(255,255,255,0.10);border-radius:10px;}");
	//ui.pushButton_devinfo->setStyleSheet("background:rgba(255,255,255,0.10);border-radius:10px;");
	modelindex = ct_dr;
	

#pragma region qss
	ui.pushButton_modeldr->setText(QString::fromLocal8Bit("DR模式      "));
	ui.pushButton_modelct->setText(QString::fromLocal8Bit("CT模式      "));
	change_user_button->setStyleSheet("QPushButton::hover{background:#666666;}QPushButton{qproperty-icon:url(:/img/res/statusbar/changeuser.png); border-radius:8px; background:rgba(255,255,255,0.10); color:#FFFFFF; }");
	ui.pushButton->setStyleSheet("QPushButton::hover{background:#666666;}QPushButton{qproperty-icon:url(:/img/res/statusbar/changeuser.png); border-radius:8px; background:rgba(255,255,255,0.10); color:#FFFFFF; }");
	ui.pushButton_ipsmanage->setStyleSheet("QPushButton::hover{background:#666666;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/manage.png);}QPushButton{color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/manage.png);}");
#pragma endregion

	connect(this, SIGNAL(ChangeView(View)), parent, SLOT(OnChangeView(View)));
	connect(ui.pushButton_modeldr, SIGNAL(clicked()), this, SLOT(OnDRView()));
	connect(ui.pushButton_modelct, SIGNAL(clicked()), this, SLOT(OnCTView()));
	connect(ui.pushButton_modeldrct, SIGNAL(clicked()), this, SLOT(OnMainView()));
	connect(ui.pushButton_ipsmanage, &QPushButton::clicked, this, &IPSStatusBar::OnIpsManageButtonClicked);
	connect(ui.pushButton_recalllast, &QPushButton::clicked, this, &IPSStatusBar::OnRecallButtonClicked);
	OnDrCtButtonClicked();
    auto pMainWidget = m_spContainer->GetWidget<MainViewWidget>(ksMainViewWidget);
	m_pIPSManager = new IPSManagerDlg(m_spContainer, pMainWidget);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::IPSStatusBar");
}

/*-----------------------------------------------------------
***   Description:     重绘事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);
    painter.fillRect(rect(), QColor(68, 68, 68));

}
/*-----------------------------------------------------------
***   Description:     切换到管理界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::showmanagement()
{

    if (!bInit)
    {
        m_pIPSManager->AttachView();
        bInit = true;
    }
	m_pIPSManager->ClearImage();
    m_pIPSManager->showFullScreen();  
	
}

/*-----------------------------------------------------------
***   Description:     显示包裹ID
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::ChangeCurrentPackagkseID(QString id)
{
	id.insert(17, ' ');
	id.insert(9, ' ');
	ui.label_currentid->setText(id);
}

/*-----------------------------------------------------------
***   Description:     CT界面切换
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnCtButtonClicked()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSStatusBar::OnCtButtonClicked");
	modelindex = ct;
	ui.pushButton_modelct->setStyleSheet("QPushButton{background:white;color:#626262;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CT.png);}");
	ui.pushButton_modeldrct->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CTDR.png);}");
	ui.pushButton_modeldr->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/DR.png);}");
	UpdateButtonStyleSheet();
	emit VertiDisable();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSStatusBar::OnCtButtonClicked");
}

/*-----------------------------------------------------------
***   Description:     DR界面切换
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnDrButtonClicked()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnDrButtonClicked");
	modelindex = dr;
	ui.pushButton_modeldr->setStyleSheet("QPushButton{background:white;color:#626262;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/DR.png);}");
	ui.pushButton_modelct->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CT.png);}");
	ui.pushButton_modeldrct->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CTDR.png);}");
	UpdateButtonStyleSheet();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnDrButtonClicked");
}

/*-----------------------------------------------------------
***   Description:     主界面切换
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnDrCtButtonClicked()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnDrCtButtonClicked");
	modelindex = ct_dr;
	ui.pushButton_modeldrct->setStyleSheet("QPushButton{background:white;color:#626262;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CTDR.png);}");
	ui.pushButton_modelct->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF;border:0px groove gray; border-radius:10px;}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/CT.png);}");
	ui.pushButton_modeldr->setStyleSheet("QPushButton::hover{background:#666666;color:#FFFFFF;border:0px groove gray; border-radius:10px;}QPushButton{background:transparent;color:#FFFFFF;border:0px groove gray; border-radius:10px;qproperty-icon:url(:/img/res/statusbar/DR.png);}");
	UpdateButtonStyleSheet();	
	emit VertiDisable();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnDrCtButtonClicked");
}

/*-----------------------------------------------------------
***   Description:     IPS管理界面切换
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnIpsManageButtonClicked()
{
	commandQueueMutex.lock();
	commandQueue.push(10);
	commandQueueMutex.unlock();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnIpsManageButtonClicked");
	emit ChangeView(View::ManageVIEW);
}

/*-----------------------------------------------------------
***   Description:     更新按钮样式表
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::UpdateButtonStyleSheet()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::updateButtonStyleSheet");
	ui.pushButton_modelct->style()->polish(ui.pushButton_modelct);
	ui.pushButton_modeldr->style()->polish(ui.pushButton_modeldr);
	ui.pushButton_modeldrct->style()->polish(ui.pushButton_modeldrct);
}

/*-----------------------------------------------------------
***   Description:     更新时间显示
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::showtime()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::showtime");
	QDateTime time = QDateTime::currentDateTime();
	QString str = time.toString("yyyy-MM-dd hh:mm:ss");
	ui.label_time->setText(str);
}

/*-----------------------------------------------------------
***   Description:     设置当前tip状态
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::setTipStatus(tipStatus status)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::setTipStatus");
	tipstatus = status;
}

/*-----------------------------------------------------------
***   Description:     切换至CT界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnCTView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnCTView");
	OnCtButtonClicked();
	emit ChangeView(View::CTVIEW);
}

/*-----------------------------------------------------------
***   Description:     切换至DR界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnDRView()
{
	OnDrButtonClicked();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnDRView");
	emit ChangeView(View::DRVIEW);
}

/*-----------------------------------------------------------
***   Description:     切换至DRCT界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::OnMainView()
{
	OnDrCtButtonClicked();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::OnMainView");
	emit ChangeView(View::MainView);
}

/*-----------------------------------------------------------
***   Description:     切换tip状态
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::changeTipStatus(tipStatus status)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSStatusBar::changeTipStatus");
	
	if (tipstatus == tipOn && status == tipOff)
	{
		ui.label_tipstatus->setText("未开启");
		tipstatus = tipOff;
	}
	else if (tipstatus == tipOff && status == tipOn)
	{
		ui.label_tipstatus->setText("TIP已开启");
		tipstatus = tipOn;
	}
}

/*-----------------------------------------------------------
***   Description:     设置用户ID
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSStatusBar::SetUserID(QString id)
{
	ui.label_comuserid->setText(id);
}

void IPSStatusBar::ChangeCheckStatus(bool check)
{
	if (check)
	{
		packagesNum++;
		ui.label_packagesnum->setText(QString::fromLocal8Bit("共") + QString::number(packagesNum) + QString::fromLocal8Bit("个"));
		if(manage_permission)
			ui.pushButton_ipsmanage->setDisabled(true);
	}
	else
	{
		if(manage_permission)
		ui.pushButton_ipsmanage->setDisabled(false);
	}
}
void IPSStatusBar::OnRecallButtonClicked()
{
	emit Recall();
}
void IPSStatusBar::OnResetButtonClicked()
{
	emit StopRecall();
}
IPSStatusBar::~IPSStatusBar()
{
	delete m_pIPSManager;
}
