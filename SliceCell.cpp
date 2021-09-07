#include "SliceCell.h"
#include "CustomStyleImage.h"  
#include "vtkImageData.h"
#include "qdebug.h"
#include "DisplayPickerValueWidget.h"
#include "vtkTextProperty.h"
#include "vtkProp.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"


SliceCell::SliceCell(ContainerPtr spContainer, QWidget * pMainWnd)
    :Cell2D(spContainer, pMainWnd)
{
   
    m_picker = vtkSmartPointer<vtkCellPicker>::New();
    m_picker->SetTolerance(0.05);
    m_spInteractor->SetPicker(m_picker);
    if (m_spStyleImage != NULL)
    {
        m_spStyleImage->m_picker = m_picker;
        // m_spImageViewer->SetColorLevel(1000);
         //m_spImageViewer->SetColorWindow(1000);
        m_spStyleImage->m_updateStatisticFun = std::bind(&SliceCell::UpdateStatisticInfo,
            this, std::placeholders::_1);
    }

}

SliceCell::~SliceCell()
{
}
void SliceCell::Initialize()
{
    __super::Initialize();
    //auto pPickerValue = m_spContainer->GetWidget<QWidget>(ksDisplayPickerValueWidget);
    //const QObject* pSend = (const QObject*)m_spStyleImage.Get();
    //connect(pSend, SIGNAL(Location(QVector<int>)), this, SLOT(Location(QVector<int>)));
}
//**************************************************
//	brief   :更新统计信息 
//	in      :当前选择的点信息
//	out     :
//	return  : 
//**************************************************
void SliceCell::Location(QVector<int> vecPos)
{
    int pos[3] = { vecPos[0],vecPos[1],vecPos[2] };

    auto iPixelValue = GetPixelVaue(pos);
    auto pPickerValueWidget = m_spContainer->GetWidget< DisplayPickerValueWidget>(ksDisplayPickerValueWidget);
    
    pPickerValueWidget->UpdateStatisticInfo(vector<short>(iPixelValue));

    //pPickerValueWidget->UpdatePixelValue(iPixelValue);
}
//**************************************************
//	brief   :更新统计信息 
//	in      :当前选择的点信息
//	out     :
//	return  : 
//**************************************************
void SliceCell::UpdateStatisticInfo(const vector<Point3D> &&vecPoint)
{
    if (vecPoint.size() == 0)
    {
        return;
    }
    vector<short> vecPixel;
    vecPixel.reserve(vecPoint.size());
    for (auto item : vecPoint)
    {
        int pos[3] = { item.m_x, item.m_y, item.m_z };
        auto iPixelValue = GetPixelVaue(pos);
        vecPixel.push_back(iPixelValue);
    }
    auto pPickerValueWidget = m_spContainer->GetWidget< DisplayPickerValueWidget>(ksDisplayPickerValueWidget);
    pPickerValueWidget->UpdateStatisticInfo(std::move(vecPixel));
}

void SliceCell::SetIsCTMode(bool bIsCT)
{
	m_bCTMode = bIsCT;
}
void SliceCell::FitWindow()
{
	//m_spImageViewer->Render();
	//return;
	if (nullptr == m_spDisplayImageData)
	{
		return;
	}
	m_spImageViewer->UpdateDisplayExtent();


	//auto size = m_pMainWnd->frameGeometry();
	auto *pVtkSize = m_spImageViewer->GetSize();
	int w = pVtkSize[0];
	int h = pVtkSize[1];

	auto spRender = m_spImageViewer->GetRenderer();
	vtkCamera* pCamera = spRender->GetActiveCamera();
	int extent[6];
	m_spDisplayImageData->GetExtent(extent);

	double origin[3];
	m_spDisplayImageData->GetOrigin(origin);

	double spacing[3];
	m_spDisplayImageData->GetSpacing(spacing);

	float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
	float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
	//  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
	float yd_y = (extent[3] - extent[2] + 1)*spacing[1];
	float xd = (extent[1] - extent[0] + 1)*spacing[0];

 

	auto yd_x = xd * h / w;  //计算充满X轴时候的高度值
	auto yd = std::max(yd_y, yd_x);//计算充满Y轴时候的高度值
	if (m_bCTMode)
	{
		yd *= 1.5;
	}
	pCamera->SetParallelScale(0.5f*static_cast<float>(yd));
	auto scale2 = pCamera->GetParallelScale();

	float d = pCamera->GetDistance();
	pCamera->SetFocalPoint(xc, yc, 0.0);
	pCamera->SetPosition(xc, yc, +d);
	UpdateRuler();
	m_spImageViewer->Render();

}
