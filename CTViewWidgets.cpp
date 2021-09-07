#include "CTViewWidgets.h"
#include "Cell2D.h"
#include "Cell3D.h"
#include "SliceCell.h"
extern int dim[3];
/*-----------------------------------------------------------
***   Description:     CT界面构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
CTViewWidgets::CTViewWidgets(ContainerPtr spContainer, QWidget *parent )
    : BaseView(spContainer, parent)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CTViewWidgets::CTViewWidgets");
    ui.setupUi(this);
	verti = ui.verticalSlider;
	hori = ui.horizontalSlider;
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


	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnHorizontalSlideChange(int)));
	connect(ui.verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnVerticalSliderChange(int)));
}

/*-----------------------------------------------------------
***   Description:     CT界面析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
CTViewWidgets::~CTViewWidgets()
{


	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CTViewWidgets::~CTViewWidgets");
}

/*-----------------------------------------------------------
***   Description:     设置CT界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void CTViewWidgets::AttachView()
{


	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CTViewWidgets::AttachView");
   /* auto pTopStatus = m_spContainer->GetWidget<QWidget>(ksTopStatusWidget);
    ui.gridLayout_topStatusBar->removeWidget(pTopStatus);
    ui.gridLayout_topStatusBar->addWidget(pTopStatus);                     
*/
   /* auto pBottomStatus = m_spContainer->GetWidget<QWidget>(ksStatusWidget);
    ui.gridLayout_BottomStatusBar->removeWidget(pBottomStatus);
    ui.gridLayout_BottomStatusBar->addWidget(pBottomStatus);*/
    auto pToolBarWidget = m_spContainer->GetWidget<QWidget>(ksToolBarWidget);
    ui.verticalLayout_toolbar->removeWidget(pToolBarWidget);
    ui.verticalLayout_toolbar->addWidget(pToolBarWidget);

    auto pCheckResultWidget = m_spContainer->GetWidget<QWidget>(ksCheckResultWidget);
    ui.gridLayout_CheckResult->removeWidget(pCheckResultWidget);
    ui.gridLayout_CheckResult->addWidget(pCheckResultWidget);

    auto pDisplayPickerValueWidget = m_spContainer->GetWidget<QWidget>(ksDisplayPickerValueWidget);
    ui.gridLayout_picker->removeWidget(pDisplayPickerValueWidget);
    ui.gridLayout_picker->addWidget(pDisplayPickerValueWidget);

    auto pSliceCell = m_spContainer->GetWidget<SliceCell>(ksSliceCellWidget);
	pSliceCell->SetIsCTMode(true);
    ui.gridLayout_sliceView->removeWidget(pSliceCell);
    ui.gridLayout_sliceView->addWidget(pSliceCell);
    //pSliceCell->Initialize();
    pSliceCell->FitWindow();

    auto pResultView = m_spContainer->GetWidget<QWidget>(ksResultWidget);
    ui.gridLayout_resultview->removeWidget(pResultView);
    ui.gridLayout_resultview->addWidget(pResultView);

    auto p3DCel = m_spContainer->GetWidget<QWidget>(ks3DCellWidget);
    ui.gridLayout_3dView->removeWidget(p3DCel);
    ui.gridLayout_3dView->addWidget(p3DCel);

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


	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CTViewWidgets::AttachView");
}

/*-----------------------------------------------------------
***   Description:    水平滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void CTViewWidgets::OnHorizontalSlideChange(int iPos)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "CTViewWidgets::OnHorizontalSlideChange");
	/* auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
	 pSliceCell->PageTra(iPos);
	 pSliceCell->FitWindow();*/
	auto p3dCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
	p3dCell->PageTra(iPos);
	auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
	pSliceCell->PageTra(iPos);
	// emit PageTra(iPos);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "CTViewWidgets::OnHorizontalSlideChange");
}


/*-----------------------------------------------------------
***   Description:    垂直滑块拖动
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void  CTViewWidgets::OnVerticalSliderChange(int iPos)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "CTViewWidgets::OnVerticalSliderChange");
	//auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
	//pOriCell->PageSag(iPos);
	//pOriCell->FitWindow();
	auto p3dCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
	p3dCell->PageSag(iPos);
	auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
	pSliceCell->PageSag(iPos);
	//emit PageSag(iPos);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "CTViewWidgets::OnVerticalSliderChange");
}

void CTViewWidgets::DisableVerti()
{
	ui.verticalSlider->setDisabled(true);
}