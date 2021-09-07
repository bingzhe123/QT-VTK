#include "DRCell.h"
#include "vtkPNGReader.h"
#include "vtkButtonWidget.h"
#include "vtkProp3DButtonRepresentation.h"
/*-----------------------------------------------------------
***   Description:     DRCell构造函数
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/

class vtkButtonCallback : public vtkCommand
{
public:
	static vtkButtonCallback *New()
	{
		return new vtkButtonCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkButtonWidget *buttonWidget =	reinterpret_cast<vtkButtonWidget*>(caller);
		vtkProp3DButtonRepresentation *rep = reinterpret_cast<vtkProp3DButtonRepresentation *>(buttonWidget->GetRepresentation());
		int state = rep->GetState();
		m_pCell->OnChangeDrDisplay();
	}
	vtkButtonCallback()
	{}
	DRCell *m_pCell = nullptr;
};

DRCell::DRCell(ContainerPtr spContainer, QWidget * pMainWnd)
	:Cell2D(spContainer, pMainWnd)
{

	m_btnImg1 = vtkSmartPointer<vtkImageData>::New();
	m_btnImg2 = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkPNGReader> pngReader1 = vtkSmartPointer<vtkPNGReader>::New();
	pngReader1->SetFileName("res/1.png");
	pngReader1->Update();
	vtkSmartPointer<vtkPNGReader> pngReader2 = vtkSmartPointer<vtkPNGReader>::New();
	pngReader2->SetFileName("res/2.png");
	pngReader2->Update();
	m_btnImg1 = pngReader1->GetOutput();
	m_btnImg2 = pngReader2->GetOutput();
	m_buttonRepresentation = vtkSmartPointer<vtkTexturedButtonRepresentation2D>::New();
	m_buttonRepresentation->SetNumberOfStates(2);
	m_buttonRepresentation->SetButtonTexture(0, m_btnImg1);
	m_buttonRepresentation->SetButtonTexture(1, m_btnImg2);

	vtkSmartPointer<vtkButtonCallback> callback = vtkSmartPointer<vtkButtonCallback>::New();
	callback->m_pCell = this;
	vtkButtonWidget *buttonWidget = vtkButtonWidget::New();
	buttonWidget->SetInteractor(m_spInteractor);
	buttonWidget->SetRepresentation(m_buttonRepresentation);
	buttonWidget->AddObserver(vtkCommand::StateChangedEvent, callback);
	buttonWidget->SetEnabled(1);
	buttonWidget->On();
}


void DRCell::showEvent(QShowEvent *event)
{
	__super::showEvent(event);

	int dim[3];
	m_btnImg2->GetDimensions(dim);

	vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetCoordinateSystemToNormalizedDisplay();
	coordinate->SetValue(0, 1);
	double bds[6];
	double sz = 135;
	bds[0] = coordinate->GetComputedDisplayValue(m_pRender)[0];
	bds[1] = bds[0] + 50;
	bds[2] = coordinate->GetComputedDisplayValue(m_pRender)[1];
	bds[3] = bds[2] + 50;
	bds[4] = bds[5] = 0.0;
	m_buttonRepresentation->PlaceWidget(bds);
}

//**************************************************
//	brief   :button的点击槽函数 
//	in      :
//	out     :
//	return  : 
//**************************************************
void DRCell::OnChangeDrDisplay()
{
	if (!HasDRData())
	{
		return;
	}
    if (m_bDR)
    {//照片
        string sPath = "res/real_image.png";
        LoadPng(sPath);    
    }
    else
    {//DR 图片
        //string sPath = "res/DR.png";
        //LoadPng(sPath);
		ShowDR();
    }
	m_bDR = !m_bDR;
}

void DRCell::NegativeProcess()
{

}
