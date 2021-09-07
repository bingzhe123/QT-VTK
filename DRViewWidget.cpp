#include "DRViewWidget.h"
#include "IPSAPP.h"
#include "Cell2D.h"



/*-----------------------------------------------------------
***   Description:     DR界面构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
DRViewWidget::DRViewWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DRViewWidget::DRViewWidget");
    m_pApp = (IPSAPP*)parent;
    ui.setupUi(this);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DRViewWidget::DRViewWidget");
}

/*-----------------------------------------------------------
***   Description:     DR界面析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
DRViewWidget::~DRViewWidget()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DRViewWidget::~DRViewWidget");
}

/*-----------------------------------------------------------
***   Description:     粘贴界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void DRViewWidget::AttachView()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DRViewWidget::AttachView");
    //auto spContainer = m_spContainer;
    //auto pTopStatus = spContainer->GetWidget<QWidget>(ksTopStatusWidget);
    //ui.gridLayout_topStatus->removeWidget(pTopStatus);
    //ui.gridLayout_topStatus->addWidget(pTopStatus);
    //auto pBottomStatus = spContainer->GetWidget<QWidget>(ksStatusWidget);
    //ui.horizontalLayout_BottomStatusBar->removeWidget(pBottomStatus);
    //ui.horizontalLayout_BottomStatusBar->addWidget(pBottomStatus);

    auto childrenList = ui.horizontalLayout_cell->children();
    auto pOriCell = m_spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    pOriCell->FitWindow();
    ui.horizontalLayout_cell->removeWidget(pOriCell);
    ui.horizontalLayout_cell->addWidget(pOriCell);
    pOriCell->update();


	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DRViewWidget::AttachView");
}
