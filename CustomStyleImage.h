/*----------------------------------------------------------------------------
   Component       :CustomStyleImage
   Name            :CustomStyleImage
   Author          :
   Creation Date   : 2021.05.30
   Description     : 点拾取交互类
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/

#ifndef CUSTOMSTYLEIMAGE_H_
#define CUSTOMSTYLEIMAGE_H_

#include "vtkInteractorStyleImage.h"
#include "vtkImageViewer2.h"
#include "vtkImageData.h"
#include "vtkCellPicker.h"
#include "vtkSmartPointer.h"
#include "vtkLineSource.h"
#include "qobject.h"
#include <vector>
#include <functional>
#include "commondef.h"
#include "Cell2D.h"

using std::vector;
using UpdateStatisticFunc = std::function<void(const vector<Point3D> &&)>;
using UpdateZoomFunc = std::function<void(float)>;
using HideRulerFunc = std::function<void(void)>;

#include <vtkInteractorStyleRubberBand3D.h>
class CustomStyleImage :
    public QObject, public vtkInteractorStyleRubberBand3D
{
    Q_OBJECT
public:
    vtkTypeMacro(CustomStyleImage, vtkInteractorStyleRubberBand3D);
    virtual ~CustomStyleImage();
    static CustomStyleImage* New();

    //**************************************************
    //	brief   :重载基类鼠标操作 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void OnLeftButtonDown();
    virtual void OnRightButtonDown();
    virtual void OnLeftButtonUp();

    void SetLine(int index, double point1[], double point2[]);

    void ComputeSelectRegion(int startPos[2], int endPos[2]);
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
    float GetZoom(vtkRenderer *R, vtkImageData *I);
Q_SIGNALS :
    void Location(QVector<int> vecPos);

public:
    vtkSmartPointer<vtkImageViewer2> m_spImageViewer = nullptr;
    vtkSmartPointer<vtkCellPicker>  m_picker;
    //矩形的4条线
    vtkSmartPointer<vtkActor> m_vecLineActor[4];
    vtkSmartPointer<vtkLineSource> m_vecLineSource[4];
    UpdateStatisticFunc m_updateStatisticFun = nullptr;
    UpdateZoomFunc m_updateZoomFun = nullptr;
    HideRulerFunc m_HideRulerFun = nullptr;
    Cell2D *m_pCurrentCell;

};


#endif // ! CUSTOMSTYLEIMAGE_H_
