/*----------------------------------------------------------------------------
   Component       : 
   Name            :
   Author          :
   Creation Date   : 
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CUSTOM_STYPE_TRACKBALL_H_
#define CUSTOM_STYPE_TRACKBALL_H_

#include "vtkInteractorStyleTrackballCamera.h"
#include "qwidget.h"
#include "vtkCellPicker.h"
#include "vtkSmartPointer.h"
#include "vtkRenderWindowInteractor.h"
#include <qdebug.h>
#include "vtkActor.h"
#include "vtkLineSource.h"
#include "vtkInteractorStyleRubberBand3D.h"
#include "vtkBoxWidget2.h"
#include "vtkBoxCallback.h"
#include "vtkBoxRepresentation.h"

#include "gespips.h"

struct BoxWidgetItem
{
	vtkSmartPointer<vtkBoxWidget2> m_spBoxWidget;
	vtkSmartPointer<vtkBoxRepresentation> m_spBoxRep;
	vtkSmartPointer<vtkBoxCallback> m_spBoxCallback;
	bool m_isUsed = false;
	bool m_isSelect = false;
	QString m_uid;//每个框选的可疑物的唯一标识
};
using UpdateZoomFunc = std::function<void(float)>;
class Cell;
class CustomStyleTrackballCamera :
    public vtkInteractorStyleRubberBand3D
{
public:
    vtkTypeMacro(CustomStyleTrackballCamera, vtkInteractorStyleRubberBand3D);
    virtual ~CustomStyleTrackballCamera();
    static CustomStyleTrackballCamera* New();

    CustomStyleTrackballCamera();
    void OnDoubleClicked();

    void SetLine(int index, double point1[], double point2[]);

    void Update();
    float GetZoom(vtkRenderer *R);

    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseMove() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
	double GetRulerLength();

    virtual void Dolly(double factor);

public:
    Cell *m_pCurrentCell = nullptr;
    vtkSmartPointer<vtkVolume> m_pVolume = nullptr;
    vtkSmartPointer<vtkImageData> m_spImageData;
    unsigned int m_numberOfClicks = 0;
    int m_previousPosition[2];
    int m_resetPixelDistance;
    UpdateZoomFunc m_updateZoomFun = nullptr;
    double m_dDefaultDistance = 0;
    double m_bounds[6];


    int m_iStartPos[3];
    int m_iCurrentPos[3];

	int m_iRulerStart[3];
	int m_iRulerEnd[3];
	double m_dRulerStartWord[3];
	double m_dRulerEndWord[3];
	bool m_bInRuler = false;

    // 矩形的4条线
    vtkSmartPointer<vtkActor> m_vecLineActor[4];
    vtkSmartPointer<vtkLineSource> m_vecLineSource[4];
    double m_defualtBounds[6];
    double m_selectBounds[6];
	bool m_bCanCropping = false;
	bool m_bMeasure = false;
	bool m_bMeasureStartInVolume = false;//开始点是否在体内
	bool m_bMeasureEndInVolume = false;//结束点是否在体内

	std::shared_ptr<BoxWidgetItem> m_spBoxItem;

    bool m_bClickAxis = false; //是否点击坐标轴
    int m_iCroppingCount = 0;

};
#endif //  CUSTOM_STYPE_TRACKBALL_H_



