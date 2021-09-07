#include "CTAndDRViewWidget.h"
#include "IPSAPP.h"
#include "Cell2D.h"
#include "Cell3D.h"

CTAndDRViewWidget::CTAndDRViewWidget(ContainerPtr spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
    ui.setupUi(this);
}

CTAndDRViewWidget::~CTAndDRViewWidget()
{
}

void CTAndDRViewWidget::AttachView()
{
    auto spContainer = m_spContainer;
    auto pTopStatus = spContainer->GetWidget<QWidget>(ksTopStatusWidget);
   // ui.gridLayout_topStatus->removeWidget(pTopStatus);
    //ui.gridLayout_topStatus->addWidget(pTopStatus);
    auto pBottomStatus = spContainer->GetWidget<QWidget>(ksStatusWidget);
   // ui.horizontalLayout_BottomStatusBar->removeWidget(pBottomStatus);
    //ui.horizontalLayout_BottomStatusBar->addWidget(pBottomStatus);

    auto pResultView = spContainer->GetWidget<QWidget>(ksResultWidget);
    ui.gridLayout_result->removeWidget(pResultView);
    ui.gridLayout_result->addWidget(pResultView);

    auto pOriCell = spContainer->GetWidget<Cell2D>(ksOriginalCellWidget);
    auto pSliceCell = spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
    auto p3dCell = spContainer->GetWidget<QWidget>(ks3DCellWidget);
    ui.gridLayout_oriView->removeWidget(pOriCell);
    ui.gridLayout_oriView->addWidget(pOriCell);
    ui.gridLayout_sliceView->removeWidget(pSliceCell);
    ui.gridLayout_sliceView->addWidget(pSliceCell);
    ui.gridLayout_3dView->removeWidget(p3dCell);
    ui.gridLayout_3dView->addWidget(p3dCell);
    pOriCell->FitWindow();
    pSliceCell->FitWindow();
    p3dCell->update();
    pOriCell->update();
    pSliceCell->update();
}