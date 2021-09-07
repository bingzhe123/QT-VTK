#include "MainViewWidget.h"
#include "IPSAPP.h"
#include "Cell2D.h"
#include "Cell3D.h"
#include "SliceCell.h"
#include "DataModule.h"

int dim[3];

/*-----------------------------------------------------------
***   Description:    主窗口构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
MainViewWidget::MainViewWidget(ContainerPtr spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::MainViewWidget");

    m_pApp = (IPSAPP*)parent;
	ui.setupUi(this);
#pragma region slider qss
		ui.verticalSlider->setStyleSheet("  \
		QSlider::add-page:Vertical{background-color:#D9D9D9;width:2px;}\
		QSlider::sub-page:Vertical{background-color:#D9D9D9;width:2px;}\
		QSlider::groove:Vertical{background:transparent;width:2px;}\
		QSlider::handle{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/verti.png);margin: 0 -10px;}\
		QSlider::handle:hover{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/vertih.png);margin: 0 -10px;}\
		QSlider::handle:pressed{height: 22px;width:25px;border-image:url(:/img/res/mainwindow/vertic.png);margin: 0 -10px;}\
		");
		//QSlider{background-image:url(:/img/res/mainwindow/white.png);}
		ui.horizontalSlider->setStyleSheet("QSlider::add-page:Horizontal{background-color:#D9D9D9;height:2px;}\
         QSlider::sub-page:Horizontal{background-color:#D9D9D9;height:2px;}\
		QSlider::groove:Horizontal{background:transparent;height:2px;}\
        QSlider::handle{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizon.png);margin: -10 0px;}\
		QSlider::handle:hover{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizonh.png);margin: -10 0px;}\
		QSlider::handle:pressed{height: 25px;width:22px;border-image: url(:/img/res/mainwindow/horizonc.png);margin: -10 0px;}\
		");

		hori = ui.horizontalSlider;
		verti = ui.verticalSlider;


#pragma endregion
		//ui.frame->setObjectName("toolbar");
		//ui.frame->setStyleSheet("#toolbar{background-image:url(:/img/res/white.png);border:0px;};");
		//toolbar = new IPSToolsBar(ui.frame);
		//toolbar->setFixedWidth(49);

    /*
1、这里的#号表示，主控件不会影响子控件
2、设置多个样式，可以用双引号和分号
*/
   /* setStyleSheet("#QWidget{background-color:white;"
        "border-width:1px;"
        "border-style:solid;"
        "border-color:white}");*/ 
  connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnHorizontalSlideChange(int)));
  connect(ui.verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVerticalSliderChange(int)));
  LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::MainViewWidget");
}

/*-----------------------------------------------------------
***   Description:    主窗口析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
MainViewWidget::~MainViewWidget()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::~MainViewWidget");
}

/*-----------------------------------------------------------
***   Description:    粘贴窗口
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void MainViewWidget::AttachView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::AttachView");
    auto spContainer = m_spContainer;
   /* auto pTopStatus = spContainer->GetWidget<QWidget>(ksTopStatusWidget);
    ui.gridLayout_topStatus->removeWidget(pTopStatus);
    ui.gridLayout_topStatus->addWidget(pTopStatus);*/
    //auto pStatusWidget = spContainer->GetWidget<QWidget>(ksStatusWidget);
    ////pStatusWidget->setStyleSheet("#IPSStatusBar{background-color:white;"
    ////                              "border-width:1px;"
    ////                              "border-style:solid;"
    ////    "border-color:white}");

    //ui.gridLayout_navigetView->removeWidget(pStatusWidget);
    //ui.gridLayout_navigetView->addWidget(pStatusWidget);
    auto pToolBarWidget = spContainer->GetWidget<QWidget>(ksToolBarWidget);
    ui.verticalLayout_toolbar->removeWidget(pToolBarWidget);
    ui.verticalLayout_toolbar->addWidget(pToolBarWidget);

    auto pResultView = spContainer->GetWidget<QWidget>(ksResultWidget);
    ui.gridLayout_result->removeWidget(pResultView);
    ui.gridLayout_result->addWidget(pResultView);
    auto pCheckResultWidget = spContainer->GetWidget<QWidget>(ksCheckResultWidget);
    ui.horizontalLayout_checkResult->removeWidget(pCheckResultWidget);
    ui.horizontalLayout_checkResult->addWidget(pCheckResultWidget);

    auto pDisplayPickerValueWidget = spContainer->GetWidget<QWidget>(ksDisplayPickerValueWidget);
    ui.gridLayout_picker->removeWidget(pDisplayPickerValueWidget);
    ui.gridLayout_picker->addWidget(pDisplayPickerValueWidget);
    
    auto pOriCell = spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    auto pSliceCell = spContainer->GetWidget<SliceCell>(ksSliceCellWidget);
	pSliceCell->SetIsCTMode(false);
    auto p3dCell = spContainer->GetWidget<QWidget>(ks3DCellWidget);
    ui.gridLayout_oriView->removeWidget(pOriCell);
    ui.gridLayout_oriView->addWidget(pOriCell);
    ui.gridLayout_sliceView->removeWidget(pSliceCell);
    ui.gridLayout_sliceView->addWidget(pSliceCell);
    ui.gridLayout_3dView->removeWidget(p3dCell);
    ui.gridLayout_3dView->addWidget(p3dCell);
	this->updateGeometry();
	this->update();
	pSliceCell->FitWindow();

    auto pCroppingCell = m_spContainer->GetWidget<Cell3D>(ksCroppintCellWidget);
    ui.gridLayout_croppingView->removeWidget(pCroppingCell);
    ui.gridLayout_croppingView->addWidget(pCroppingCell);
    pCroppingCell->hide();
    LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::AttachView");
}

/*-----------------------------------------------------------
***   Description:    载入数据
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void MainViewWidget::LoadData()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::LoadData");
    auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    auto pSliceCell = m_spContainer->GetWidget<SliceCell>(ksSliceCellWidget);
    auto p3dCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
    p3dCell->LoadData();

    auto spDataModule = m_spContainer->GetModule<DataModule>(ksDataModule);
    
    auto spImageData = spDataModule->GetOriginalImageData();
    spImageData->GetDimensions(dim);

	
	//pOriCell->LoadPng("res/DR.png");
	pOriCell->ShowDR();
	ui.verticalSlider->setMinimum(0);
	ui.verticalSlider->setMaximum(dim[0] - 1);
	auto iPos = dim[0] / 2;
	ui.verticalSlider->setSliderPosition(iPos);
	ui.verticalSlider->setPageStep(1);
    ui.horizontalSlider->setMinimum(0);
    ui.horizontalSlider->setMaximum(dim[2]-1);
    iPos = dim[2] / 2;
    ui.horizontalSlider->setSliderPosition(iPos);
    ui.horizontalSlider->setPageStep(1);
    pSliceCell->PageTra(iPos);

    pOriCell->FitWindow();
    pSliceCell->FitWindow();
    p3dCell->update();
    pOriCell->update();
    pSliceCell->update();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::LoadData");
}


/*-----------------------------------------------------------
***   Description:    水平滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void MainViewWidget::OnHorizontalSlideChange(int iPos)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::OnHorizontalSlideChange");
   /* auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
    pSliceCell->PageTra(iPos);
    pSliceCell->FitWindow();*/
	auto p3dCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
	p3dCell->PageTra(iPos);
	auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
	pSliceCell->PageTra(iPos);
   // emit PageTra(iPos);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::OnHorizontalSlideChange");

}

/*-----------------------------------------------------------
***   Description:    垂直滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void MainViewWidget::OnVerticalSliderChange(int iPos)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::OnVerticalSliderChange");
    //auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    //pOriCell->PageSag(iPos);
    //pOriCell->FitWindow();
	auto p3dCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
	p3dCell->PageSag(iPos);
	auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
	pSliceCell->PageSag(iPos);
	//emit PageSag(iPos);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "MainViewWidget::OnVerticalSliderChange");
}

void MainViewWidget::DisableVerti()
{
	ui.verticalSlider->setDisabled(true);
}