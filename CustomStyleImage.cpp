#include "CustomStyleImage.h"
#include <qdebug.h>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRendererCollection.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkCellPicker.h"
#include "vtkLineSource.h"
#include "vtkCamera.h"
#include "commondef.h"
#include "vtkImageActor.h"
#include <vtkInteractorStyleTrackballCamera.h>

CustomStyleImage::~CustomStyleImage()
{

}
void CustomStyleImage::OnLeftButtonDown()
{
    return __super::OnLeftButtonDown();
}
void CustomStyleImage::OnRightButtonDown()
{
   
}
void CustomStyleImage::SetLine(int index, double point1[], double point2[])
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
        auto spRender = m_spImageViewer->GetRenderer();
        spRender->AddActor(actor);
        m_vecLineActor[index] = actor;
        m_vecLineSource[index] = lineSource;
    }
    m_vecLineSource[index]->SetPoint1(point1);
    m_vecLineSource[index]->SetPoint2(point2);
    m_vecLineSource[index]->Update();
    m_spImageViewer->Render();
}
void CustomStyleImage::OnLeftButtonUp()
{ 
    __super::OnLeftButtonUp();
    if (m_spImageViewer == nullptr || m_picker == nullptr)
    {
        return ;
    }
    auto startPos = this->GetStartPosition();
    auto endPos = this->GetEndPosition();
    if (fabs(startPos[0] - endPos[0])<1e-1)
    {
        endPos[0] = startPos[0] + 1;
    }

    if (fabs(startPos[1] - endPos[1]) < 1e-1)
    {
        endPos[1] = startPos[1] + 1;
    }

    double dStartPos[3] = { startPos[0],startPos[1],0 };
    auto spRender = m_spImageViewer->GetRenderer();
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
    ComputeSelectRegion(startPos, endPos);
   
}
void CustomStyleImage::ComputeSelectRegion(int start[2], int end[2])
{
    //直接计算2个顶点的坐标，然后遍历
    m_spImageViewer->UpdateDisplayExtent();
    auto spRender = m_spImageViewer->GetRenderer();
    m_picker->Pick(start[0], start[1], 0, spRender);
    double pos[3];
    m_picker->GetPickPosition(pos);
    int iImgStart[3]; ;
    m_picker->GetPointIJK(iImgStart);
    m_picker->GetPCoords();

    m_picker->Pick(end[0], end[1], 0, spRender);
    m_picker->GetPickPosition(pos);
    int iImgEnd[3]; ;
    m_picker->GetPointIJK(iImgEnd);

    vector<Point3D> vecPoint;
    int x1 = iImgStart[0] <= iImgEnd[0] ? iImgStart[0] : iImgEnd[0];
    int y1 = iImgStart[1] <= iImgEnd[1] ? iImgStart[1] : iImgEnd[1];

    int x2 = iImgStart[0] > iImgEnd[0] ? iImgStart[0] : iImgEnd[0];
    int y2 = iImgStart[1] > iImgEnd[1] ? iImgStart[1] : iImgEnd[1];

    

    int w = x2 - x1 + 1;
    int h = y2 - y1 + 1;
    vecPoint.reserve(w*h);

    for (int j = y1; j <= y2; j++)
    {
        for (size_t i = x1; i <= x2; i++)
        {
            vecPoint.emplace_back(i, j, 0);
        }
    }
    if (m_updateStatisticFun != nullptr)
    {
        m_updateStatisticFun(std::move(vecPoint));
    }
}

float CustomStyleImage::GetZoom(vtkRenderer *pRender, vtkImageData *pImageData)
{
    int *dim = pImageData->GetDimensions();
    vtkSmartPointer<vtkCoordinate> c = vtkCoordinate::New();
    double *pSpace = pImageData->GetSpacing();
    c->SetCoordinateSystemToWorld();
    auto hh = dim[1] * pSpace[1];
    c->SetValue(0, hh, 0);
    auto res = c->GetComputedDisplayValue(pRender);
    auto endPos = res[1];
    c->SetValue(0, 0, 0);
    res = c->GetComputedDisplayValue(pRender);
    auto oriPos = res[1];

    auto *pVtkSize = m_spImageViewer->GetSize();
    int w = pVtkSize[0];
    int h = pVtkSize[1];
    auto fscale = float(endPos - oriPos) / h;
    qDebug() << fscale;
  
    return fscale;
}
void CustomStyleImage::OnMouseWheelForward()
{
    __super::OnMouseWheelForward();
    auto spRender = m_spImageViewer->GetRenderer();
    auto spImageData = m_pCurrentCell->GetImageData();
    auto fzoom = GetZoom(spRender,spImageData);
    if (m_updateZoomFun != nullptr)
    {
        m_updateZoomFun(fzoom);
    }
}
void CustomStyleImage::OnMouseWheelBackward()
{
    if (m_HideRulerFun != nullptr)
    {
        m_HideRulerFun();
    }
    __super::OnMouseWheelBackward();
    auto spRender = m_spImageViewer->GetRenderer();
    auto spImageData = m_pCurrentCell->GetImageData();
    auto fzoom = GetZoom(spRender, spImageData);
    if (m_updateZoomFun != nullptr)
    {
        m_updateZoomFun(fzoom);
    }
}
