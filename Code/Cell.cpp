#include "Cell.h"
#include <qdebug>
#include "vtkTextProperty.h"
#include "vtkRenderer.h"
#include "vtkProperty2D.h"


/*-----------------------------------------------------------
***   Description:     子界面构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Cell::Cell(ContainerPtr spContainer, QWidget * pMainWnd)
{   

	
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::Cell");
    m_spContainer = spContainer;
    m_spInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    m_spCornerAnnotationBottomRight = vtkSmartPointer< vtkCornerAnnotation>::New();
    auto sText = QStringLiteral("Zoom: ");
    sText = sText + QString::number(1.00, 'f', 2);
    m_spCornerAnnotationBottomRight->SetText(vtkCornerAnnotation::LowerRight, sText.toStdString().c_str());
    //TextProperty->SetFontFile("RegularScript.ttf");//楷体
    vtkTextProperty *pTextProperty = vtkTextProperty::New();
    pTextProperty->SetFontFamily(VTK_FONT_FILE);
    pTextProperty->SetFontSize(8);
    m_spCornerAnnotationBottomRight->SetTextProperty(pTextProperty);
    m_spCornerAnnotationBottomRight->GetTextProperty()->SetColor(1, 0, 0);
    m_spCornerAnnotationBottomRight->VisibilityOff();

    m_spAxis = vtkSmartPointer<MyVtkAxisActor2D>::New();
	m_spAxis->SetLabelsNumber(11);
    m_spAxis->SetTickLength(20);
    m_spAxis->SetTickOffset(10);
    m_spAxis->SetTitle("mm");
    m_spAxis->SetMinorTickLength(9);
    m_spAxis->SetTitlePosition(1);
	m_spAxis->SetLabelFormat("%0.0f");
	m_spAxis->GetProperty()->SetColor(1, 0, 0);
	m_spAxis->SetFontFactor(0.75);

	m_spAxis->AdjustLabelsOff();
	//m_spAxis->SetNumberOfLabels(10);
    double dy = 0.98;
    double p0[2] = { 0.1,dy };
    double p1[2] = { 0.9,dy };
    m_spAxis->SetPoint1(p0);
    m_spAxis->SetPoint2(p1);
	m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::Cell");
}

/*-----------------------------------------------------------
***   Description:     子界面析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Cell::~Cell()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::~Cell");
    m_spInteractor->Disable();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::~Cell");
}

/*-----------------------------------------------------------
***   Description:     停止接口
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Cell::StopInteractor()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::StopInteractor");
    m_spInteractor->ExitEvent();
    m_spInteractor->ExitCallback();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::StopInteractor");
}

/*-----------------------------------------------------------
***   Description:     定位
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Cell::Location(double ptPos[3])
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell::Location");
}

void Cell::PageTra(int iIndex)
{
}

void Cell::PageSag(int iIndex)
{
}

void Cell::ClearIamge()
{
}

vtkSmartPointer<vtkImageData> Cell::GetImageData()
{
    return vtkSmartPointer<vtkImageData>();
 }

//**************************************************
//	brief   :重载窗口显示的消息，用来设置vtk的interaction的窗口大小
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell::showEvent(QShowEvent *event)
{
    if (m_spInteractor->GetRenderWindow() == nullptr)
    {
        return;
    }
    int *pSize = m_spInteractor->GetRenderWindow()->GetActualSize();

    qDebug() << "showEvent";
    qDebug() << "showEvent:ActiveSize:" << pSize[0] << "  " << pSize[1];

    auto rect = this->geometry();
    qDebug() << "showEvent:geometry:" << rect.width() << "  " << rect.height();
    //m_spInteractor->UpdateSize(pSize[0], pSize[1]);
    m_spInteractor->UpdateSize(rect.width(), rect.height());
}


/*-----------------------------------------------------------
***   Description:    窗口变化函数
***   OUTPUT     :
***   INPUT      : event：消息内容
------------------------------------------------------------*/
void Cell::resizeEvent(QResizeEvent * event)
{
    auto rect = this->geometry();
    m_spInteractor->UpdateSize(rect.width(), rect.height());

}

/*-----------------------------------------------------------
***   Description: 更新zoom值
***   OUTPUT     :
***   INPUT      :fZoom：当前的zoom值
------------------------------------------------------------*/
void Cell::UpdateZoom(float fZoom)
{
    auto sText = QStringLiteral("Zoom: ");
    sText = sText + QString::number(fZoom, 'f', 2);
    m_spCornerAnnotationBottomRight->SetText(vtkCornerAnnotation::LowerRight, sText.toStdString().c_str());
    //m_pRender->Render();
    UpdateRuler();
   // m_pRender->GetRenderWindow()->Render();
	emit UpdateScaling(fZoom);
}

/*-----------------------------------------------------------
***   Description:获取在当前显示窗口下每个像素表示的物理长度
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
double Cell::GetPixelSize()
{
    return 0.0;
}
void Cell::SetDataModule(DataModule * pDataModule)
{
	m_pDataModule = pDataModule;
}
bool Cell::HasDRData()
{
	return m_pDataModule != nullptr&&m_pDataModule->GetDrOriginalImageData() != nullptr;
}
bool Cell::HasCTData()
{
	return m_pDataModule != nullptr&&m_pDataModule->GetOriginalImageData() != nullptr;
}
/*-----------------------------------------------------------
***   Description: 更新比例尺
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Cell::UpdateRuler()
{
	if (!m_spAxis->GetVisibility())
	{
		return;
	}
    auto pSize = m_pRender->GetRenderWindow()->GetSize();
    int iLength;
    int iRulerDistance;
    m_ruler.GetParameter(pSize[0], pSize[1], GetPixelSize(), iLength, iRulerDistance);
    m_spAxis->SetRange(0, iRulerDistance);
    if (iLength< pSize[0]/2)
    {
        m_spAxis->SetNumberOfMinorTicks(4);

    }
    else
    {
        m_spAxis->SetNumberOfMinorTicks(9);
    }

    double iStart = (pSize[0] - iLength)/2;
    double pos1 = (double)iStart / pSize[0];
    double pos2 = (double)(pSize[0] + iLength) / 2.0 / pSize[0];
    double dy = 0.99;

    double p0[2] = { pos1,dy };  //比例尺端点的位置.x,y(0-1)
    double p1[2] = { pos2,dy };
    m_spAxis->SetPoint1(p0);
    m_spAxis->SetPoint2(p1);
    m_spAxis->RenderOverlay(m_pRender);    
    m_pRender->GetRenderWindow()->Render();
   // m_pRender->Render();
}

void Cell::HideRuler()
{
    m_ruler.Hide();
}
