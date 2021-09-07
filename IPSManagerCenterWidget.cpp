#include "IPSManagerCenterWidget.h"
#include "IPSAPP.h"
extern IPSThreadReciver *reciver;
bool init = false;
extern QMutex commandQueueMutex;
extern std::queue<int > commandQueue;
IPSManagerCenterWidget::IPSManagerCenterWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
    ui.setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags & ~Qt::FramelessWindowHint );
    //ui.pushButton_ImageMabager->setStyleSheet("border-image:url(:/img/res/titleseot.png);");
   // setAttribute(Qt::WA_TranslucentBackground);
    //setObjectName("IPSManagerCenterWidget");
    //setStyleSheet("background-color:#444444;");
    //auto pImageWidget = new IPSManagerImageWidget(spContainer, parent);
    //m_spContainer->AddWidget(ksIPSManagerImageWidget, pImageWidget);
    //ui.verticalLayout_ImageDisplay->addWidget(pImageWidget);
	auto spDataModule = new DataModule();
	m_spContainer->AddModule(ksIspManagerDataModule, spDataModule);

    auto pIpsManagerDRCell = new DRCell(m_spContainer, this);
    m_spContainer->AddWidget(ksIspManagerOriginalCellWidget, pIpsManagerDRCell);
    auto spImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    pIpsManagerDRCell->SetImageViewer(spImageViewer);


    auto pIpsManagerSliceCell = new SliceCell(m_spContainer, this);
    m_spContainer->AddWidget(ksIspManagerSliceCellWidget, pIpsManagerSliceCell);
    auto spImageViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
    pIpsManagerSliceCell->SetImageViewer(spImageViewer2);

    auto pIpsManager3DCell = new Cell3D(m_spContainer, this);
    m_spContainer->AddWidget(ksIspManager3DCellWidget, pIpsManager3DCell);
    auto pIpsManagerCroppintCell = new Cell3D(m_spContainer, this, true);
    m_spContainer->AddWidget(ksIspManagerCroppintCellWidget, pIpsManagerCroppintCell);
	pIpsManagerCroppintCell->setVisible(false);
    auto pIpsManagerToolBar = new IPSToolsBar(m_spContainer, pIpsManager3DCell,
		pIpsManagerDRCell, pIpsManagerSliceCell, spDataModule, this);

    m_spContainer->AddWidget(ksIspManagerToolBarWidget, pIpsManagerToolBar);

    auto pIpsManagerCheckResult = new ManageCheckViewWidget(m_spContainer, this);
    m_spContainer->AddWidget(ksIspManagerCheckResultWidget, pIpsManagerCheckResult);
	
	auto pIpsResultView = new ResultViewWidgets(m_spContainer, this,false);
	ui.gridLayout->addWidget(pIpsResultView);

	auto statusWidget = new ManageStatus(this); ;
	pIpsManagerDRCell->SetDataModule(spDataModule);
	pIpsManagerSliceCell->SetDataModule(spDataModule);
	pIpsManager3DCell->SetDataModule(spDataModule);
	pIpsManagerCroppintCell->SetDataModule(spDataModule);

	ui.verticalSlider->setStyleSheet("  \
		QSlider::add-page:Vertical{background-color:#D9D9D9;width:2px;}\
		QSlider::sub-page:Vertical{background-color:#D9D9D9;width:2px;}\
		QSlider::groove:Vertical{background:transparent;width:2px;}\
		QSlider::handle{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/verti.png);margin: 0 -10px;}\
		QSlider::handle:hover{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/vertih.png);margin: 0 -10px;}\
		QSlider::handle:pressed{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/vertic.png);margin: 0 -10px;}\
		");

	ui.horizontalSlider->setStyleSheet("QSlider::add-page:Horizontal{background-color:#D9D9D9;height:2px;}\
         QSlider::sub-page:Horizontal{background-color:#D9D9D9;height:2px;}\
		QSlider::groove:Horizontal{background:transparent;height:2px;}\
        QSlider::handle{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizon.png);margin: -10 0px;}\
		QSlider::handle:hover{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizonh.png);margin: -10 0px;}\
		QSlider::handle:pressed{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizonc.png);margin: -10 0px;}\
		");
	ui.horizontalSlider->setEnabled(false);
	connect(pIpsManagerCheckResult->vertiRadio, &QRadioButton::clicked, this, [=] {ui.horizontalSlider->setEnabled(false); ui.verticalSlider->setEnabled(true); });
	connect(pIpsManagerCheckResult->horiRadio, &QRadioButton::clicked, this, [=] {ui.horizontalSlider->setEnabled(true); ui.verticalSlider->setEnabled(false); });
	ui.verticalLayout->addWidget(statusWidget);
	connect(reciver->decoder, &IPSThreadDecoder::ReviewListRecived, statusWidget, &ManageStatus::InsertReviewList);
	connect(pIpsManager3DCell, &Cell3D::AddSuspicious, pIpsResultView, &ResultViewWidgets::AddItemToSuspiciousContainer);
	connect(reciver->decoder, &IPSThreadDecoder::ReviewSusRecived, pIpsResultView, &ResultViewWidgets::ShowReview);
	connect(statusWidget, &ManageStatus::SelectPacage, pIpsResultView, &ResultViewWidgets::SetReviewID);
	connect(statusWidget, &ManageStatus::SelectPacage, pIpsManagerCheckResult, &ManageCheckViewWidget::ChangeID);
	connect(pIpsResultView, &ResultViewWidgets::ChangeLockStatus, pIpsManagerCheckResult, &ManageCheckViewWidget::ChangeLockStatus);
	connect(reciver->decoder, &IPSThreadDecoder::ReviewStart, statusWidget, &ManageStatus::InsertReviewUser);
	connect(this,&IPSManagerCenterWidget::DisableButton ,statusWidget, &ManageStatus::Clear);
	connect(statusWidget, &ManageStatus::ShowSessionID, this, &IPSManagerCenterWidget::DownloadShow);
	connect(this, &IPSManagerCenterWidget::ClearOprator, statusWidget, &ManageStatus::ClearOprator);
	connect(this, &IPSManagerCenterWidget::ClearItems, statusWidget, &ManageStatus::ClearItems);
    connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnHorizontalSlideChange(int)));
    connect(ui.verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVerticalSliderChange(int)));
	connect(pIpsManagerCheckResult, &ManageCheckViewWidget::DeleteOne, this, [=] 
	{
		QList<QListWidgetItem*> selectedItems = statusWidget->list_area->selectedItems();
		if (selectedItems.count() > 0)
		{
			foreach(QListWidgetItem* var, selectedItems)
			{
				statusWidget->list_area->removeItemWidget(var);
				selectedItems.removeOne(var);
				delete var;
			}
		}
		OnClearDicomImages();
	});
 }

IPSManagerCenterWidget::~IPSManagerCenterWidget()
{                    
}

//**************************************************
//	brief   :重载resizeEvent 
//	in      :QResizeEvent：envet
//	out     :
//	return  : 
//**************************************************
void IPSManagerCenterWidget::resizeEvent(QResizeEvent *event)
{
    auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksIspManagerSliceCellWidget);
    auto rc = pOriCell->geometry();
    auto size = pOriCell->m_spRenderWindow->GetScreenSize();
    auto activesize = pOriCell->m_spRenderWindow->GetActualSize();
}


/*-----------------------------------------------------------
***   Description:    粘贴窗口
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSManagerCenterWidget::AttachView()
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSManagerCenterWidget::AttachView");
    auto spContainer = m_spContainer;
    auto pToolBarWidget = spContainer->GetWidget<QWidget>(ksIspManagerToolBarWidget);
    ui.verticalLayout_toolbar->removeWidget(pToolBarWidget);
    ui.verticalLayout_toolbar->addWidget(pToolBarWidget);

    auto pResultView = spContainer->GetWidget<QWidget>(ksIspManagerResultWidget);
    ui.gridLayout_result->removeWidget(pResultView);
    ui.gridLayout_result->addWidget(pResultView);

    auto pCheckResultWidget = spContainer->GetWidget<QWidget>(ksIspManagerCheckResultWidget);
    ui.horizontalLayout_checkResult->removeWidget(pCheckResultWidget);
    ui.horizontalLayout_checkResult->addWidget(pCheckResultWidget);

    auto pOriCell = spContainer->GetWidget<DRCell>(ksIspManagerOriginalCellWidget);
    auto pSliceCell = spContainer->GetWidget<SliceCell>(ksIspManagerSliceCellWidget);
    auto p3dCell = spContainer->GetWidget<Cell3D>(ksIspManager3DCellWidget);
    
    ui.gridLayout_oriView->removeWidget(pOriCell);
    ui.gridLayout_oriView->addWidget(pOriCell);
  /*  string sPath2 = "res/DR.png";
    pOriCell->LoadPng(sPath2);*/

    auto size = pSliceCell->size();
    //auto cell = new Cell2D(m_spContainer, this);
    /*auto spImageViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
    pSliceCell->SetImageViewer(spImageViewer2);*/

	/*pSliceCell->m_spRenderWindow = pRenderWindow2;
    pSliceCell->SetRenderWindow(pRenderWindow2)*/;
    ui.gridLayout_sliceView->removeWidget(pSliceCell);
    ui.gridLayout_sliceView->addWidget(pSliceCell);
  /*  string sPath = "res/DR.png";
    pSliceCell->LoadPng(sPath);*/

    //ui.qvtkWidget_sliceView->SetRenderWindow(pSliceCell->m_spRenderWindow);

    ui.gridLayout_3dView->removeWidget(p3dCell);
    ui.gridLayout_3dView->addWidget(p3dCell);
   // ui.qvtkWidget_3dView->SetRenderWindow(p3dCell->m_spRenderWindow);

   // auto pCroppingCell = m_spContainer->GetWidget<Cell3D>(ksIspManagerCroppintCellWidget);
   ///* ui.gridLayout_croppingView->removeWidget(pCroppingCell);
   // ui.gridLayout_croppingView->addWidget(pCroppingCell);*/
   // ui.qvtkWidget_croppingView->SetRenderWindow(pCroppingCell->m_spRenderWindow);
    //pCroppingCell->hide();
    LOG_DEBUG(GESP::DebugLogger::GetInstance(),"MainViewWidget::AttachView");
}

void IPSManagerCenterWidget::Close()
{
	emit ClearOprator();
	emit ClearItems();
	emit DisableButton();
	commandQueueMutex.lock();
	commandQueue.push(14);
	commandQueueMutex.unlock();
}


void IPSManagerCenterWidget::DownloadShow(QString sessionid)
{
	OnClearDicomImages();
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "创建CT图像下载文件夹失败");
		return;
	}

	try
	{
		DicomCMove(sessionid.toStdString(), 2); // 2为CT图像
		LOG_INFO(GESP::DebugLogger::GetInstance(), "CT图像下载完成");
	}
	catch (...)
	{

	}
	if (!isDirExist(QString(ksDownloadFold.c_str())))
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "创建DR图像下载文件夹失败");
		return;
	}
	DicomCMove(sessionid.toStdString(), 1); // 1为DR图像
	LOG_INFO(GESP::DebugLogger::GetInstance(), "DR图像下载完成");
	
	OnLoadDicomImages(sessionid);
}
 
/*-----------------------------------------------------------
***   Description : 判断文件夹是否存在，不存在则创建
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
bool IPSManagerCenterWidget::isDirExist(QString fullPath)
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
***   Description : 载入下载的dicom图像
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSManagerCenterWidget::OnLoadDicomImages(QString session_id)
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnLoadDicomImages");
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSAPP::OnLoadDicomImages");
	std::string folder = ksDownloadFold;
	folder.append("\\");
	folder.append(session_id.toStdString().c_str());

	std::string CTFolder(folder);
	CTFolder.append("\\CT");
	std::string DRFolder(folder);
	DRFolder.append("\\DR");

    auto pAPP = m_spContainer->GetWidget<IPSAPP>(ksAppWidget);
	if (!pAPP->DividCTAndDR(folder, CTFolder, DRFolder))
	{
		LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Divide dicom images failed !");
		
	}

	auto spImportImagesData = std::make_shared< ImportImagesData>(CTFolder.c_str());
	auto spImageData = spImportImagesData->GetImageData();
	//std::string file = "..//code//test_data//multiframe//Heart.dcm";
	//auto spImageData = spImportImagesData->ReadMultiFrameImage(file.c_str());

	int dim[3];
	spImageData->GetDimensions(dim);
	if (dim[0] <= 0 || dim[1] <= 0 || dim[2] <= 0)
	{
		LOG_INFO(GESP::DebugLogger::GetInstance(), "载入CT图像失败");
		QMessageBox::information(NULL, "tip", QString::fromLocal8Bit("载入CT图像失败"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
    auto spDataModule = m_spContainer->GetModule<DataModule>(ksIspManagerDataModule);

    spDataModule->SetRawDataPacket(spImageData);

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
    spDataModule->SetDrRawDataPacket(spImageData);



    auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksIspManagerOriginalCellWidget);
    auto pSliceCell = m_spContainer->GetWidget<SliceCell>(ksIspManagerSliceCellWidget);
    auto p3dCell = m_spContainer->GetWidget<Cell3D>(ksIspManager3DCellWidget);
    p3dCell->LoadData();
    auto spOriImageData = spDataModule->GetOriginalImageData();
    spOriImageData->GetDimensions(dim);


    //pOriCell->LoadPng("res/DR.png");
    pOriCell->ShowDR();
    ui.verticalSlider->setMinimum(0);
    ui.verticalSlider->setMaximum(dim[0] - 1);
    auto iPos = dim[0] / 2;
    ui.verticalSlider->setSliderPosition(iPos);
    ui.verticalSlider->setPageStep(1);
    ui.horizontalSlider->setMinimum(0);
    ui.horizontalSlider->setMaximum(dim[2] - 1);
    iPos = dim[2] / 2;
    ui.horizontalSlider->setSliderPosition(iPos);
    ui.horizontalSlider->setPageStep(1);
    pSliceCell->PageTra(iPos);

    pOriCell->FitWindow();
    pSliceCell->FitWindow();
    p3dCell->update();
    pOriCell->update();
    pSliceCell->update();
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnLoadDicomImages");
}
/*-----------------------------------------------------------
***   Description : 清除管理界面的图像
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSManagerCenterWidget::OnClearDicomImages()
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnClearDicomImages");

    auto pIpsManagerDRCell = m_spContainer->GetWidget<DRCell>(ksIspManagerOriginalCellWidget);
	if(pIpsManagerDRCell)
    pIpsManagerDRCell->ClearIamge();
	
    auto pIpsManagerSliceCell = m_spContainer->GetWidget<SliceCell>(ksIspManagerSliceCellWidget);
	if (pIpsManagerSliceCell)
    pIpsManagerSliceCell->ClearIamge();

    auto pIpsManager3DCell = m_spContainer->GetWidget<Cell3D>(ksIspManager3DCellWidget);
	if(pIpsManager3DCell)
    pIpsManager3DCell->ClearIamge();
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnClearDicomImages");

}


/*-----------------------------------------------------------
***   Description:    水平滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSManagerCenterWidget::OnHorizontalSlideChange(int iPos)
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnHorizontalSlideChange");
    /* auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
     pSliceCell->PageTra(iPos);
     pSliceCell->FitWindow();*/
    auto p3dCell = m_spContainer->GetWidget<Cell3D>(ksIspManager3DCellWidget);
    p3dCell->PageTra(iPos);
    auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksIspManagerSliceCellWidget);
    pSliceCell->PageTra(iPos);
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnHorizontalSlideChange");

}

/*-----------------------------------------------------------
***   Description:    垂直滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSManagerCenterWidget::OnVerticalSliderChange(int iPos)
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnVerticalSliderChange");
    //auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    //pOriCell->PageSag(iPos);
    //pOriCell->FitWindow();
    auto p3dCell = m_spContainer->GetWidget<Cell3D>(ksIspManager3DCellWidget);
    p3dCell->PageSag(iPos);
    auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksIspManagerSliceCellWidget);
    pSliceCell->PageSag(iPos);

    LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSManagerCenterWidget::OnVerticalSliderChange");
}

void IPSManagerCenterWidget::showEvent(QEvent * event)
{
	OnClearDicomImages();
}
