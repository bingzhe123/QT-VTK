#include "IPSAPP.h"

extern std::queue<int> commandQueue;
extern QMutex commandQueueMutex;
extern std::queue<std::string> commandContainer;
extern QMutex commandContainerMutex;
extern int MachineMode;
extern TCPClient instruct;
extern IPSThreadReciver *reciver;
extern QString userID;
extern std::string sessionidrecall[100];//回溯列表
extern int recalllocate;			 //回溯列表定位符
int recallnumber = 0;			 //当前回溯图片
std::string recallid;
std::queue<QString> sessionidlist;
bool recallMode = true;
/*-----------------------------------------------------------
***   Description:     主框架构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
IPSAPP::IPSAPP(QWidget *parent)
    : QMainWindow(parent)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::IPSAPP");
    ui.setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    QPalette pal(this->palette());

    //QColor bk(70, 70,70);
    //pal.setColor(QPalette::Background,bk); //设置背景黑色
    //setAutoFillBackground(true);
    //setPalette(pal);
	//showFullScreen();
	CT_downloaded = false;
	DR_downloaded = true;
	if (MachineMode)
		showMaximized();
	else
		showFullScreen();

	m_spDataModule = new DataModule();
#pragma region UI初始化
    m_spContainer = std::make_shared<Container>();
    m_spContainer->AddWidget(ksAppWidget,this);

    auto pMainViewWidget = new MainViewWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksMainViewWidget, pMainViewWidget);  
    //this->setCentralWidget(pMainViewWidget);

    auto pDRCell = new DRCell(m_spContainer, this);
    m_spContainer->AddWidget(ksOriginalCellWidget, pDRCell);
    auto pSliceCell = new SliceCell(m_spContainer, this);
    m_spContainer->AddWidget(ksSliceCellWidget, pSliceCell);
    auto p3DCell = new Cell3D(m_spContainer, this);
    m_spContainer->AddWidget(ks3DCellWidget, p3DCell);

    auto pCroppintCell = new Cell3D(m_spContainer, this, true);
    m_spContainer->AddWidget(ksCroppintCellWidget, pCroppintCell);
             
    auto pDRViewWidget = new DRViewWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksDRViewWidget, pDRViewWidget);    

    auto pCTViewWidget = new CTViewWidgets(m_spContainer, this);
    m_spContainer->AddWidget(ksCTViewWidget, pCTViewWidget);

    auto pTopStatusWidgeet = new TopStateBarWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksTopStatusWidget, pTopStatusWidgeet);
    
    auto pStatusWidget = new IPSStatusBar(m_spContainer, this);
    m_spContainer->AddWidget(ksStatusWidget, pStatusWidget);

    auto pResultWidget = new ResultViewWidgets(m_spContainer, this);
    m_spContainer->AddWidget(ksResultWidget, pResultWidget);

    auto pIspManagerResultWidget = new ResultViewWidgets(m_spContainer, this);
    m_spContainer->AddWidget(ksIspManagerResultWidget, pIspManagerResultWidget);

    auto pToolBar = new IPSToolsBar(m_spContainer, p3DCell, pDRCell, pSliceCell, m_spDataModule, this );
    m_spContainer->AddWidget(ksToolBarWidget, pToolBar);

    auto pCheckResult = new CheckResultWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksCheckResultWidget, pCheckResult);


    auto pDisplayPickerValueWidget = new DisplayPickerValueWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksDisplayPickerValueWidget, pDisplayPickerValueWidget);

#pragma endregion     
    m_spContainer->AddModule(ksDataModule, m_spDataModule);
	pDRCell->SetDataModule(m_spDataModule);
	pSliceCell->SetDataModule(m_spDataModule);
	p3DCell->SetDataModule(m_spDataModule);
	pCroppintCell->SetDataModule(m_spDataModule);



    ui.left_frame->setFixedWidth(20);
    ui.left_frame->setFixedHeight(320);

    ui.frame_right->setFixedWidth(20);
    ui.frame_right->setFixedHeight(320);
    ui.gridLayout_topstatus->addWidget(pTopStatusWidgeet);
    ui.gridLayout_statusBar->addWidget(pStatusWidget);
   // m_pBkPixmap1 = new QPixmap(":/img/res/bk.png");
    setObjectName("MainWND");
	
	statusbar = pStatusWidget;
	checkarea = pCheckResult;
 /*   setStyleSheet("#MainWND{background-color:white;"
        "border-width:1px;"
        "border-style:solid;"
        "border-color:white}");*/

    setStyleSheet("QMainWindow#MainWND{background-color:white}");  

	Change2MainView();
	if (MachineMode)
	{
		std::string folder = "..//code//test_data//dcm";
		//std::string folder = "..//code//test_data//test";

		auto spImportImagesData = std::make_shared< ImportImagesData>(folder.c_str());
		auto spImageData = spImportImagesData->GetImageData();
		//std::string file = "..//code//test_data//multiframe//Heart.dcm";
		//auto spImageData = spImportImagesData->ReadMultiFrameImage(file.c_str());
		m_spDataModule->SetRawDataPacket(spImageData);

		// 设置dr数据
		std::string file = "..//code//test_data//newDR//dr_a.dcm";
		spImageData = spImportImagesData->ReadDRImage(file.c_str());
	    //spImageData = spImportImagesData->ReadMultiFrameImage(file.c_str());

		//spImageData = spImportImagesData->GetImageData();
		//vtkSmartPointer<vtkImageData> dest;
		//m_spDataModule->ModifyDrRawData(spImageData,dest);
		
		//m_spDataModule->SetDrRawDataPacket(dest);
		//folder = "..//code//test_data//testDR";
		//spImportImagesData = std::make_shared<ImportImagesData>(folder.c_str());
		//spImageData = spImportImagesData->GetImageData();
		m_spDataModule->SetDrRawDataPacket(spImageData);
		pMainViewWidget->LoadData();
	} 
   

        /* double dSpacex, dSpacey, dSpacez;
         m_spImportImagesData->GetImagesData(m_pData, m_w, m_h, m_z, dSpacex, dSpacey, dSpacez);
         m_spImageData = m_spImportImagesData->GetImageData();
         m_pData = (short *) m_spImageData->GetScalarPointer();*/
	ui.centralWidget->setObjectName("Canvas");
	ui.centralWidget->setStyleSheet("#Canvas{background:#0;}");
	MainWidget = pMainViewWidget;
	CTWidgets = pCTViewWidget;
	auto pDeviceInfo = new IPSDeviceInformation();
	tipstimer = new QTimer(this);
    connect(pMainViewWidget, SIGNAL(PageTra(int)), p3DCell, SLOT(PageTra(int)));
    connect(pMainViewWidget, SIGNAL(PageSag(int)), p3DCell, SLOT(PageSag(int)));

    connect(pMainViewWidget, SIGNAL(PageTra(int)), pSliceCell, SLOT(PageTra(int)));
    connect(pMainViewWidget, SIGNAL(PageSag(int)), pSliceCell, SLOT(PageSag(int)));

	connect(pStatusWidget->change_user_button, &QPushButton::clicked, this, &IPSAPP::ChangeUserSignal);
	connect(pStatusWidget->deviceInfo, &QPushButton::clicked, this, [=] {pDeviceInfo->showFullScreen(); });

	connect(this, &IPSAPP::StartCheck, pCheckResult, &CheckResultWidget::StartCheck);

	connect(pResultWidget, &ResultViewWidgets::ClearIamge, pDRCell, &Cell::ClearIamge);
	connect(pResultWidget, &ResultViewWidgets::ClearIamge, pSliceCell, &Cell::ClearIamge);
	connect(pResultWidget, &ResultViewWidgets::ClearIamge, p3DCell, &Cell::ClearIamge);
	connect(pResultWidget, &ResultViewWidgets::ClearIamge, pCroppintCell, &Cell::ClearIamge);

	//connect(tipstimer, SIGNAL(timeout()), this, SLOT(tipscheck()));

	connect(reciver->decoder, &IPSThreadDecoder::GetCTImageSignal, this, &IPSAPP::DownloadCTImage);
	connect(reciver->decoder, &IPSThreadDecoder::GetDRImageSignal, this, &IPSAPP::DownloadDRImage);

	connect(reciver->decoder, &IPSThreadDecoder::StartManualCheckSignal, this, &IPSAPP::StartManualCheck);
	connect(this, &IPSAPP::ChangeEquipIDSignal, pStatusWidget, &IPSStatusBar::ChangeCurrentPackagkseID);
	connect(pToolBar,  &IPSToolsBar::ChangeNegtiveStatus, pTopStatusWidgeet,&TopStateBarWidget::ContrastColor);
	connect(checkarea, &CheckResultWidget::AutoCheckSwitch, pTopStatusWidgeet, &TopStateBarWidget::ChangeAIShow);
	connect(checkarea, &CheckResultWidget::AutoCheckSwitch, pResultWidget, &ResultViewWidgets::ChangeAIShow);

	connect(checkarea, &CheckResultWidget::SetAICounts, pResultWidget, &ResultViewWidgets::SetAICount);
	connect(checkarea, &CheckResultWidget::HoriCheck, this, &IPSAPP::GetHoriSignal);
	connect(checkarea, &CheckResultWidget::VertiCheck, this, &IPSAPP::GetVertiSignal);

	connect(pToolBar, &IPSToolsBar::Color, pTopStatusWidgeet, &TopStateBarWidget::Color);
	connect(pToolBar, &IPSToolsBar::Gray, pTopStatusWidgeet, &TopStateBarWidget::Black);
	connect(pToolBar, &IPSToolsBar::High, pTopStatusWidgeet, &TopStateBarWidget::High);
	connect(pToolBar, &IPSToolsBar::Low, pTopStatusWidgeet, &TopStateBarWidget::Low);
	connect(pToolBar, &IPSToolsBar::Edge, pTopStatusWidgeet, &TopStateBarWidget::Edge);
	connect(pToolBar, &IPSToolsBar::Organic, pTopStatusWidgeet, &TopStateBarWidget::Organic);
	connect(pToolBar, &IPSToolsBar::IC, pTopStatusWidgeet, &TopStateBarWidget::IC);
	connect(this, &IPSAPP::EnableToolsbar, pToolBar, &IPSToolsBar::SetEnable);
	connect(this, &IPSAPP::DisableToolsbar, pToolBar, &IPSToolsBar::SetDisable);
	connect(checkarea, &CheckResultWidget::DisableToolsbar, pToolBar, &IPSToolsBar::SetDisable);
	connect(this,&IPSAPP::ChangeCheckStatus,pStatusWidget, &IPSStatusBar::ChangeCheckStatus);

	connect(p3DCell, &Cell::UpdateScaling, pTopStatusWidgeet, &TopStateBarWidget::UpdateScaling);
	connect(p3DCell, &Cell3D::AddSuspicious, pResultWidget, &ResultViewWidgets::AddItemToSuspiciousContainer);

	connect(pResultWidget, &ResultViewWidgets::SelectSuspiciousObject, p3DCell, &Cell3D::SelectSuspiciousObject);
	connect(pResultWidget, &ResultViewWidgets::RemoveSuspiciousObject, p3DCell, &Cell3D::RemoveSuspiciousObject);
	connect(reciver->coder, &IPSThreadCoder::RefreshSusp, pResultWidget, &ResultViewWidgets::ClearSuspicious);
	connect(pStatusWidget, &IPSStatusBar::VertiDisable, checkarea, &CheckResultWidget::SetVertiDisable);
	connect(pStatusWidget, &IPSStatusBar::VertiDisable, pMainViewWidget, &MainViewWidget::DisableVerti);
	connect(pStatusWidget, &IPSStatusBar::VertiDisable, pCTViewWidget,&CTViewWidgets::DisableVerti);
	connect(this, &IPSAPP::StopReview, pStatusWidget->m_pIPSManager, &IPSManagerDlg::close);
	connect(checkarea, &CheckResultWidget::StopCheck, this, &IPSAPP::CheckStop);
	connect( this, &IPSAPP::StopRecallCheck,checkarea, &CheckResultWidget::StopRecallCheck );
	connect(reciver->decoder, &IPSThreadDecoder::SecondCheckRecived, this, &IPSAPP::StartSecondCheck);
	tipstimer->start(500);
	pSliceCell->Initialize();
	pStatusWidget->SetUserID(userID);
	MainWidget->hori->setEnabled(true);
	pCTViewWidget->hori->setEnabled(true);
	MainWidget->verti->setEnabled(false);
	pCTViewWidget->verti->setEnabled(false);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::IPSAPP");
}

/*-----------------------------------------------------------
***   Description:     主框架获取子界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
std::shared_ptr< Container> IPSAPP::GetContainer()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::GetContainer");
    return m_spContainer;
}


/*-----------------------------------------------------------
***   Description:     切换水平切片
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::GetHoriSignal()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Switch Horizontal Slices Mod");
	MainWidget->hori->setEnabled(true);
	CTWidgets->hori->setEnabled(true);
	MainWidget->verti->setEnabled(false);
	CTWidgets->verti->setEnabled(false);
}

/*-----------------------------------------------------------
***   Description:     切换垂直切片
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::GetVertiSignal()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Switch Vertical Slices Mod");
	MainWidget->verti->setEnabled(true);
	CTWidgets->verti->setEnabled(true);
	MainWidget->hori->setEnabled(false);
	CTWidgets->hori->setEnabled(false);
}

/*-----------------------------------------------------------
***   Description:     重绘事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::paintEvent(QPaintEvent *event)
{
	//png背景图
    //QPainter painter(this);
    //int x = 0;
    //int y = 0;
    //int w = m_pBkPixmap1->width();
    //int h = m_pBkPixmap1->height();
    //painter.drawPixmap(x, y,w, h, *m_pBkPixmap1);

	//svg背景图
	//QSvgRenderer * Converter = new QSvgRenderer(this);
	//QString strFile = ":/img/res/test.svg";
	//Converter->load(strFile);
	//QPainter p(this);
	//Converter->render(&p);
}

/*-----------------------------------------------------------
***   Description:     异步通信
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::customEvent(QEvent * event)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::customEvent");
    auto type = event->type();
    if (type == CommunicationID)
    {
        auto pEvent = (BESendMsgEvent*)event;
        BeMsgCommandHandler(pEvent->m_context);
    }    
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::customEvent");
}

/*-----------------------------------------------------------
***   Description:     关闭事件
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::closeEvent(QCloseEvent* event)
{

}

/*-----------------------------------------------------------
***   Description:  照片

***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::DownloadPhoto(QString session_id)
{
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		return;
	}

	LOG_INFO(GESP::DebugLogger::GetInstance(),"照片下载完成");

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(2);
	commandContainer.push(session_id.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
}

/*-----------------------------------------------------------
***   Description:     其它信息下载
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::DownloadOtherInfo(QString session_id)
{
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		return;
	}

	LOG_INFO(GESP::DebugLogger::GetInstance(),"其它信息下载完成");

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(2);
	commandContainer.push(session_id.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
}
/*-----------------------------------------------------------
***   Description:     图像回溯上一张
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::ChangeRecallImage()
{
	recallnumber++;
	recallid = sessionidrecall[(recalllocate - recallnumber >= 0 ? recalllocate-recalllocate: 100 + recalllocate - recallnumber)];
	recallMode = true;
	GESP::JsonObject obj;
	obj.Set(ksCommandIndex, ksSCSCommand[8]);
	obj.Set(ksID, recallid);

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(9);
	commandContainer.push(obj.Stringify());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
}
/*-----------------------------------------------------------
***   Description:     停止图像回溯
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::StopRecallImage()
{
	recallnumber=0;
	recallid = "";
	recallMode = false;
	GESP::JsonObject obj;

	commandQueueMutex.lock();
	commandQueue.push(14);
	commandQueueMutex.unlock();

	emit ChangeEquipIDSignal("");
	checkarea->eid = "";
	emit DisableToolsbar();
	emit StopRecallCheck();
}
/*-----------------------------------------------------------
***   Description:     CT图像下载
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::DownloadCTImage(QString session_id)
{
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "创建CT图像下载文件夹失败");
		return;
	}

	try
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "CT图像开始下载");
		DicomCMove(session_id.toStdString(), 2); // 2为CT图像
		LOG_INFO(GESP::DebugLogger::GetInstance(), "CT图像下载完成");
	}
	catch(...)
	{

	}

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(2);
	commandContainer.push(session_id.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
}
/*-----------------------------------------------------------
***   Description:     DR图像下载
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::DownloadDRImage(QString session_id)
{
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "创建DR图像下载文件夹失败");
		return;
	}

	LOG_INFO(GESP::DebugLogger::GetInstance(),"DR图像开始下载");
	DicomCMove(session_id.toStdString(),1); // 1为DR图像
	LOG_INFO(GESP::DebugLogger::GetInstance(), "DR图像下载完成");

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(3);
	commandContainer.push(session_id.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
}

void IPSAPP::StartManualCheck(QString sessionid)
{
	LOG_INFO(GESP::DebugLogger::GetInstance(),"开始判图  包裹ID: " + sessionid.toStdString());
	
	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(5);
	commandContainer.push(sessionid.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
	emit ChangeEquipIDSignal(sessionid);
	checkarea->eid = sessionid;
	OnLoadDicomImages(sessionid);
	emit EnableToolsbar();
	emit StartCheck(sessionid);
	emit ChangeCheckStatus(true);
	emit StopReview();
	emit ChangeCheckStatus(true);
}

void IPSAPP::StartSecondCheck(QString sessionid)
{
	LOG_INFO(GESP::DebugLogger::GetInstance(), "开始二次判图  包裹ID: " + sessionid.toStdString());

	commandQueueMutex.lock();
	commandContainerMutex.lock();
	commandQueue.push(5);
	commandContainer.push(sessionid.toStdString());
	commandContainerMutex.unlock();
	commandQueueMutex.unlock();
	emit ChangeEquipIDSignal(sessionid);
	checkarea->eid = sessionid;
	OnLoadDicomImages(sessionid);
	emit EnableToolsbar();
	emit StartCheck(sessionid);
	emit ChangeCheckStatus(false);
}

void IPSAPP::CheckStop()
{
	emit ChangeCheckStatus(false);
}

IPSAPP::~IPSAPP()
{

}

/*-----------------------------------------------------------
***   Description : 载入下载的dicom图像
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSAPP::OnLoadDicomImages(QString session_id)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSAPP::OnLoadDicomImages");
	std::string folder = ksDownloadFold;
	folder.append("\\");
	folder.append(session_id.toStdString().c_str());

	std::string CTFolder(folder);
	CTFolder.append("\\CT");
	std::string DRFolder(folder);
	DRFolder.append("\\DR");

	if (!DividCTAndDR(folder, CTFolder, DRFolder))
	{
		LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Divide dicom images failed !");
		return;
	}

	auto spImportImagesData = std::make_shared< ImportImagesData>(CTFolder.c_str());
	auto spImageData = spImportImagesData->GetImageData();
	//std::string file = "..//code//test_data//multiframe//Heart.dcm";
	//auto spImageData = spImportImagesData->ReadMultiFrameImage(file.c_str());

	int dim[3];
	spImageData->GetDimensions(dim);
	if (dim[0]<=0 || dim[1] <= 0 || dim[2]<= 0)
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "载入CT图像失败");
		QMessageBox::information(NULL, "tip", QString::fromLocal8Bit("载入CT图像失败"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_spDataModule->SetRawDataPacket(spImageData);

	// 设置dr数据
	spImportImagesData = std::make_shared<ImportImagesData>(DRFolder.c_str());
	spImageData = spImportImagesData->GetImageData();
	spImageData->GetDimensions(dim);
	if (dim[0] <= 0 || dim[1] <= 0 || dim[2] <= 0)
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "载入DR图像失败");
		QMessageBox::information(NULL, "tip", QString::fromLocal8Bit("载入DR图像失败"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	m_spDataModule->SetDrRawDataPacket(spImageData);
	
	MainWidget->LoadData();
}

/*-----------------------------------------------------------
***   Description:     切换界面事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::OnChangeView(View view)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::OnChangeView");
    switch (view)
    {
    case View::MainView:
        Change2MainView();
        break;
    case View::DRVIEW:
        Change2DRView();
        break;
    case View::CTVIEW:
        Change2CTView();
        break;
	case View::ManageVIEW:
		Change2ManageView();
		break;
    default:
        break;
    }
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::OnChangeView");
}

/*-----------------------------------------------------------
***   Description:     切换主界面2
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::Change2MainView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::Change2MainView");
    if (m_currentView == ksMainViewWidget)
    {
        return;
    }
    auto pMainWidget = m_spContainer->GetWidget<MainViewWidget>(ksMainViewWidget);

    //this->takeCentralWidget();
    pMainWidget->AttachView();
    //ui.gridLayout_mainview
   // pMainWidget->setFixedSize(1024, 768);
    pMainWidget->show();
    if (!m_currentView.empty())
    {
        auto pCurrentWidget = m_spContainer->GetWidget<QWidget>(m_currentView);
        pCurrentWidget->hide();
        ui.gridLayout_mainview->removeWidget(pCurrentWidget);
    }
    ui.gridLayout_mainview->addWidget(pMainWidget);
   // this->setCentralWidget(pMainWidget);
    m_currentView = ksMainViewWidget;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::Change2MainView");
}

/*-----------------------------------------------------------
***   Description:     切换CT界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::Change2CTView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::Change2CTView");
    if (m_currentView == ksCTViewWidget)
    {
        return;
    }
    auto pCTWidget = m_spContainer->GetWidget<CTViewWidgets>(ksCTViewWidget);
    pCTWidget->AttachView();
    pCTWidget->show();
    if (!m_currentView.empty())
    {
        auto pCurrentWidget = m_spContainer->GetWidget<QWidget>(m_currentView);
        pCurrentWidget->hide();
        ui.gridLayout_mainview->removeWidget(pCurrentWidget);
    }
   
    ui.gridLayout_mainview->addWidget(pCTWidget);
    m_currentView = ksCTViewWidget;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::Change2CTView");
}

/*-----------------------------------------------------------
***   Description:     切换DR界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::Change2DRView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSAPP::Change2DRView");
    if (m_currentView == ksDRViewWidget)
    {
        return;
    }
    auto pDRWidget = m_spContainer->GetWidget<DRViewWidget>(ksDRViewWidget);
    pDRWidget->AttachView();
    pDRWidget->show();
    if (!m_currentView.empty())
    {
        auto pCurrentWidget = m_spContainer->GetWidget<QWidget>(m_currentView);
        pCurrentWidget->hide();
        ui.gridLayout_mainview->removeWidget(pCurrentWidget);
    }
    ui.gridLayout_mainview->addWidget(pDRWidget);
    m_currentView = ksDRViewWidget;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSAPP::Change2DRView");
}

void IPSAPP::Change2ManageView()
{
	statusbar->showmanagement();
}

/*-----------------------------------------------------------
***   Description :  分类ct和dr图像，放进不同的文件夹
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
bool IPSAPP::DividCTAndDR(const string folder,
	const string ctFolder, const string drFolder)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSAPP::DividCTAndDR");
	// 判断下载文件夹是否存在
	if (!isDirExist(QString(folder.c_str())))
	{
		return false;
	}

	QDir dir(QString(folder.c_str()));
	if (!dir.exists())
	{
		return false;
	}
	dir.setFilter(QDir::Dirs | QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	// 判断文件夹是否存在，不存在则创建
	if (!isDirExist(QString(ctFolder.c_str())))
	{
		return false;
	}

	if (!isDirExist(QString(drFolder.c_str())))
	{
		return false;
	}

	for (int i = 0; i < list.size();++i)
	{
		QFileInfo fileInfo = list.at(i);
		QString name = fileInfo.fileName();
		if (name.indexOf("CT")!=-1)
		{
			QString newName(ctFolder.c_str());
			newName.append("\\");
			newName.append(fileInfo.fileName());
			if (!QFile::copy(fileInfo.filePath(),newName))
			{
				return false;
			}
			else
			{
				continue;
			}
		}

		if (name.indexOf("DP") != -1)
		{
			QString newName(drFolder.c_str());
			newName.append("\\");
			newName.append(fileInfo.fileName());
			if (!QFile::copy(fileInfo.filePath(), newName))
			{
				return false;
			}
			else
			{
				continue;
			}
		}
	}

	string srcFolder = "..\\code\\test_data\\CT";
	// 将归类后的CT文件夹里的图像测试图像代替
	InsteadSimuCT(srcFolder, ctFolder);
	srcFolder = "..\\code\\test_data\\testDR";
	// 将归类后的dr文件夹里的图像用测试图像代替
	InsteadSimuCT(srcFolder, drFolder);

	return true;
}

/*-----------------------------------------------------------
***   Description : 替换模拟器生成的CT图
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSAPP::InsteadSimuCT(const string& src, const string& dest)
{
	// 删除目标文件夹里的所有文件
	ClearAllFiles(QString(dest.c_str()));
	QDir dir(QString(src.c_str()));
	if (!dir.exists())
	{
		return;
	}
	dir.setFilter(QDir::Dirs | QDir::Files);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);
		QString newName(dest.c_str());
		newName.append("\\");
		newName.append(fileInfo.fileName());
		QFile::copy(fileInfo.filePath(), newName);
	}
}

/*-----------------------------------------------------------
***   Description : 删除文件夹里的所有文件
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSAPP::ClearAllFiles(QString path)
{
	QDir Dir(path);
	if (Dir.isEmpty())
	{
		return;
	}

	// 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
	// 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
	QDirIterator DirsIterator(path, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
	while (DirsIterator.hasNext())
	{
		if (!Dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
		{
			QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
		}
	}
}

/*-----------------------------------------------------------
***   Description : 判断文件夹是否存在，不存在则创建
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
bool IPSAPP::isDirExist(QString fullPath)
{
	QDir dir(fullPath);
	if (dir.exists())
	{
		return true;
	}
	else
	{
		bool ok = dir.mkpath(fullPath);//创建多级目录
		return ok;
	}
}

/*-----------------------------------------------------------
***   Description:     创建消息命令句柄
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSAPP::BeMsgCommandHandler(const CommandContext &context)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::BeMsgCommandHandler");
    JsonHelper helper;
    if (!helper.ParseFromString(context.sContent))
        return;
    auto id = helper.GetValue(QString("id"));
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSAPP::BeMsgCommandHandler");
}
/*-----------------------------------------------------------
***   Description:    
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/

void IPSAPP::moveEvent(QMoveEvent * event)
{
   
}

