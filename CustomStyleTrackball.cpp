#include "CustomStyleTrackball.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkVolumePicker.h"
#include "vtkImageData.h"
                                
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "Cell3D.h"
#include "vtkCamera.h"
#include "vtkMatrix4x4.h"
#include "vtkProp3DCollection.h"
#include "vtkLineWidget2.h"
#include <qdebug.h>

CustomStyleTrackballCamera::CustomStyleTrackballCamera() 
    : m_numberOfClicks(0), m_resetPixelDistance(5)
{
         
    this->m_previousPosition[0] = 0;
    this->m_previousPosition[1] = 0;
}

CustomStyleTrackballCamera::~CustomStyleTrackballCamera()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "");

}
void CustomStyleTrackballCamera::OnLeftButtonDown()
{	
    
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        __super::OnLeftButtonDown();
    }
    else
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    m_numberOfClicks++;
    this->GetInteractor()->GetEventPosition(m_iStartPos);

    int xdist = m_iStartPos[0] - this->m_previousPosition[0];
    int ydist = m_iStartPos[1] - this->m_previousPosition[1];

    this->m_previousPosition[0] = m_iStartPos[0];
    this->m_previousPosition[1] = m_iStartPos[1];

    int moveDistance = (int)sqrt((double)(xdist*xdist + ydist * ydist));

    // Reset numClicks - If mouse moved further than resetPixelDistance
    if (moveDistance > this->m_resetPixelDistance)
    {
        this->m_numberOfClicks = 1;
    }

    if (this->m_numberOfClicks == 2)
    {
        std::cout << "Double clicked." << std::endl;
        this->m_numberOfClicks = 0;
        OnDoubleClicked();
    }
	if (m_spImageData == nullptr)
	{
		return;
	}
	if (m_pCurrentCell->m_currentAction == OperationAction::ActionRuler)
	{
		m_bMeasure = true;
		memcpy(m_iRulerStart, m_iStartPos, sizeof(m_iStartPos));
		
		auto spPicker = (vtkCellPicker*)this->Interactor->GetPicker();
		auto spRender = CurrentRenderer;
		int iRes = spPicker->Pick(m_iStartPos[0], m_iStartPos[1], 0, spRender);
		if (iRes == 0)
		{//没有点在体内
			m_bMeasureStartInVolume = false;
		}
		else
		{
			m_bMeasureStartInVolume = true;
		}
		spPicker->GetPickPosition(m_dRulerStartWord);
	}

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "");
}
void CustomStyleTrackballCamera::OnMouseMove()
{

	this->GetInteractor()->GetEventPosition(m_iCurrentPos);
	if (m_pCurrentCell->m_currentAction == OperationAction::ActionRuler)
	{
		if (m_bMeasure)
		{
			memcpy(m_iRulerEnd, m_iCurrentPos, sizeof(m_iCurrentPos));

			auto spPicker = (vtkCellPicker*)this->Interactor->GetPicker();
			auto spRender = CurrentRenderer;
			auto iRes = spPicker->Pick(m_iCurrentPos[0], m_iCurrentPos[1], 0, spRender);
			if (iRes == 0)
			{
				m_bMeasureEndInVolume = false;
			}
			else
			{
				m_bMeasureEndInVolume = true;
			}
			spPicker->GetPickPosition(m_dRulerEndWord);
			//SetLine(0, m_dRulerStartWord, m_dRulerEndWord);
			((Cell3D*)m_pCurrentCell)->OnMeasure(m_iRulerStart, m_iRulerEnd, GetRulerLength());
		}
	}
    else if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        __super::OnMouseMove();
    }
    else
    {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
		int iState = this->GetState();
		if (VTKIS_ROTATE == iState)
		{
			((Cell3D*)m_pCurrentCell)->HideMeausureRuler();
		}
    }
	
   
}

void CustomStyleTrackballCamera::OnLeftButtonUp()
{
   // 使用pickerpos来取点
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        __super::OnLeftButtonUp();
        if (m_bClickAxis)
        {
            m_bClickAxis = false;
            return;
        }

        auto startPos = this->GetStartPosition();
        auto endPos = this->GetEndPosition();
        if (fabs(startPos[0] - endPos[0]) < 1e-1)
        {
            endPos[0] = startPos[0] + 1;
        }

        if (fabs(startPos[1] - endPos[1]) < 1e-1)
        {
            endPos[1] = startPos[1] + 1;
        }

        double dStartPos[3] = { startPos[0],startPos[1],0 };
        auto spRender = m_pCurrentCell->m_pRender;
        //auto orientation = spRender->GetActiveCamera()->GetOrientation();
        //spRender->SetDisplayPoint(dStartPos);
        //spRender->DisplayToView();
        //spRender->ViewToWorld();
        //double start[4];
        //spRender->GetWorldPoint(start);

        auto spPicker = (vtkCellPicker*)this->Interactor->GetPicker();
        spPicker->Pick(dStartPos[0], dStartPos[1], dStartPos[2], spRender);
        double pos1[3];
        spPicker->GetPickPosition(pos1);


		double dSEndPos[3] = { double(endPos[0]),(double)endPos[1],0.0 };
      /*  spRender->SetDisplayPoint(dSEndPos);
        spRender->DisplayToView();
        spRender->ViewToWorld();
        double end[4];
        spRender->GetWorldPoint(end);*/

        spPicker->Pick(dSEndPos[0], dSEndPos[1], dSEndPos[2], spRender);
        double pos2[3];
        spPicker->GetPickPosition(pos2);

        auto viewDirection = ((Cell3D*)m_pCurrentCell)->GetViewDirection();
        if (m_iCroppingCount == 0)
        {
			memcpy(m_selectBounds, m_defualtBounds, sizeof(m_defualtBounds));
			m_selectBounds[0] = std::min(pos1[0], pos2[0]);
            m_selectBounds[1] = std::max(pos1[0], pos2[0]);
            m_selectBounds[2] = std::min(pos1[1], pos2[1]);
            m_selectBounds[3] = std::max(pos1[1], pos2[1]);
		}
        else
        {
			//取交集
			auto x1 = std::min(pos1[0], pos2[0]);
			auto x2 = std::max(pos1[0], pos2[0]);
			auto xl = std::max(x1, m_selectBounds[0]);
			auto xr = std::min(x2, m_selectBounds[1]);
			if (xl > xr)
			{//没有交集
				xl = xr;
			}
			 
            m_selectBounds[0] = xl;
            m_selectBounds[1] = xr;
            m_selectBounds[4] = std::min(pos1[2], pos2[2]);
            m_selectBounds[5] = std::max(pos1[2], pos2[2]);
        }
       
        ((Cell3D*)m_pCurrentCell)->SelectRegion(m_selectBounds);
		if (m_pCurrentCell->m_currentAction == OperationAction::ActionPointer)
		{//SelectRegion中会改Action的状态
			m_bCanCropping = true;
		}

    }
	else if (m_pCurrentCell->m_currentAction == OperationAction::ActionRuler)
	{
	
		((Cell3D*)m_pCurrentCell)->OnMeasure(m_iRulerStart, m_iRulerEnd, GetRulerLength());
		((Cell3D*)m_pCurrentCell)->EndMeasue(false);
		m_bMeasure = false;
		vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	}
    else
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    }
    m_bClickAxis = false;
}
void CustomStyleTrackballCamera::OnMiddleButtonDown()
{
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        return  __super::OnMiddleButtonDown();
    }
    return vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}
void CustomStyleTrackballCamera::OnMiddleButtonUp()
{
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        return __super::OnMiddleButtonUp() ;
    }
    return vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}
void CustomStyleTrackballCamera::OnRightButtonDown()
{
    return;
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        return __super::OnRightButtonDown();
    }
    return vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}
void CustomStyleTrackballCamera::OnRightButtonUp()
{
    if (m_pCurrentCell->m_currentAction == OperationAction::ActionCropping)
    {
        return __super::OnRightButtonUp();
    }
    else
    {
        return vtkInteractorStyleTrackballCamera::OnRightButtonUp();
    }
}
void CustomStyleTrackballCamera::Update()
{
    auto startPos = m_iStartPos;
    auto endPos = m_iCurrentPos;
    if (fabs(startPos[0] - endPos[0]) < 1e-1)
    {
        endPos[0] = startPos[0] + 1;
    }

    if (fabs(startPos[1] - endPos[1]) < 1e-1)
    {
        endPos[1] = startPos[1] + 1;
    }

    double dStartPos[3] = { startPos[0],startPos[1],0 };
    auto spRender = m_pCurrentCell->m_pRender;
    spRender->SetDisplayPoint(dStartPos);
    spRender->DisplayToView();
    spRender->ViewToWorld();
    double start[4];
    spRender->GetWorldPoint(start);

    double dSEndPos[3] = { endPos[0],endPos[1],0 };
    spRender->SetDisplayPoint(dSEndPos);
    spRender->DisplayToView();
    spRender->ViewToWorld();
    double end[4];
    spRender->GetWorldPoint(end);

    double point1[3];
    double point2[3];
    double point3[3];
    double point4[3];

    double left[2];
    double right[2];

    left[0] = start[0] <= end[0] ? start[0] : end[0];
    left[1] = start[1] <= end[1] ? start[1] : end[1];

    right[0] = start[0] > end[0] ? start[0] : end[0];
    right[1] = start[1] > end[1] ? start[1] : end[1];

    point1[0] = left[0];  point1[1] = left[1];  point1[2] = 0;
    point2[0] = left[0];  point2[1] = right[1]; point2[2] = 0;
    point3[0] = right[0]; point3[1] = right[1]; point3[2] = 0;
    point4[0] = right[0]; point4[1] = left[1];  point4[2] = 0;

    this->SetLine(0, point1, point2);
    this->SetLine(1, point2, point3);
    this->SetLine(2, point3, point4);
    this->SetLine(3, point4, point1);
}

void CustomStyleTrackballCamera::SetLine(int index, double point1[], double point2[])
{
    if (m_vecLineActor[index] == nullptr)
    {
        vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
        lineSource->SetPoint1(point1);
        lineSource->SetPoint2(point2);
        lineSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(lineSource->GetOutputPort());
        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetLineWidth(2);
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        auto spRender = m_pCurrentCell->m_pRender;
        spRender->AddActor(actor);
        m_vecLineActor[index] = actor;
        m_vecLineSource[index] = lineSource;
    }
    m_vecLineSource[index]->SetPoint1(point1);
    m_vecLineSource[index]->SetPoint2(point2);
    m_vecLineSource[index]->Update();
    m_pCurrentCell->GetRenderWindow()->Render();
}

void CustomStyleTrackballCamera::OnDoubleClicked()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "CustomStyleTrackballCamera::OnDoubleClicked");
	//if (m_spImageData == nullptr)
	//{
	//	return;
	//}

	if (m_pCurrentCell != nullptr)
	{
		//((Cell3D*)m_pCurrentCell)->ExecuteClip();
	}

	//if (m_bCanCropping)
	//{
	//	((Cell3D*)m_pCurrentCell)->ExecuteClip();
	//	m_bCanCropping = false;
	//}
}


float CustomStyleTrackballCamera::GetZoom(vtkRenderer *pRender)
{
    auto pCamera = pRender->GetActiveCamera();
    auto distance = pCamera->GetDistance();
    qDebug() << "distance" << distance;

    auto fscale = m_dDefaultDistance/ distance;
    return fscale;
}
void CustomStyleTrackballCamera::OnMouseWheelForward()
{
    auto fTempZoom = GetZoom(CurrentRenderer);

    if (fTempZoom * 1.21 > 64.0)
    {
        return;
    }

    __super::OnMouseWheelForward();
    auto fzoom = GetZoom(CurrentRenderer);
    if (m_updateZoomFun != nullptr)
    {
        m_updateZoomFun(fzoom);
    }

}
void CustomStyleTrackballCamera::OnMouseWheelBackward()
{
    auto fTempZoom = GetZoom(CurrentRenderer);
    if (1/64.0 > fTempZoom/1.21)
    {
        return;
    }
    __super::OnMouseWheelBackward();
    auto fzoom = GetZoom(CurrentRenderer);
    if (m_updateZoomFun != nullptr)
    {
        m_updateZoomFun(fzoom);
    }
}

double CustomStyleTrackballCamera::GetRulerLength()
{
	Interactor->Modified();
	auto spPicker = (vtkCellPicker*)this->Interactor->GetPicker();
	spPicker->Modified();
	auto spRender = CurrentRenderer;
	spRender->Modified();
	spRender->GetActiveCamera()->Modified();
	m_bMeasureEndInVolume = spPicker->Pick(m_iRulerEnd[0], m_iRulerEnd[1], 0, spRender);
	spPicker->GetPickPosition(m_dRulerEndWord);

	m_bMeasureStartInVolume = spPicker->Pick(m_iRulerStart[0], m_iRulerStart[1], 0, spRender);
	spPicker->GetPickPosition(m_dRulerStartWord);
	auto x = m_dRulerEndWord[0] - m_dRulerStartWord[0];
	auto y = m_dRulerEndWord[1] - m_dRulerStartWord[1];
	auto z = m_dRulerEndWord[2] - m_dRulerStartWord[2];
	auto length = sqrt(x*x + y * y + z * z);
	return length;
}
void CustomStyleTrackballCamera::Dolly(double factor)
{
    if (this->CurrentRenderer == nullptr)
    {
        return;
    }
    //factor = 64.0;
    //int iTo64Times = 5;
    //for (int i = 0; i < iTo64Times; i++)
    //{
    //    factor = sqrtf(factor);
    //}
    auto delat = 1.414213562373095;
    if (factor<1.0)
    {
        factor = 1/ delat;
    }
    else
    {
        factor = delat;
    }
    vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
    if (camera->GetParallelProjection())
    {
        camera->SetParallelScale(camera->GetParallelScale() / factor);
    }
    else
    {
        camera->Dolly(factor);
        if (this->AutoAdjustCameraClippingRange)
        {
            this->CurrentRenderer->ResetCameraClippingRange();
        }
    }

    if (this->Interactor->GetLightFollowCamera())
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    this->Interactor->Render();
}
