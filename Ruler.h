/*----------------------------------------------------------------------------
   Component       : 比例尺
   Name            :
   Author          :
   Creation Date   : 2021.06.15
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  RULER_H_
#define  RULER_H_
#include <qpoint.h>
#include <vector>
#include "vtkLineSource.h"
#include "vtkActor.h"
#include "vtkRenderer.h"   
#include "vtkTextActor.h"
using std::vector;

struct Line
{
    QPointF ptStart;
    QPointF ptEnd;
};

class Ruler
{
public:
    Ruler();
    ~Ruler();

    /*-----------------------------------------------------------
    ***   Description:设置参数
    ***   OUTPUT     :
    ***   INPUT      :w,h窗口的尺寸，pixelSize每个像素表示的物理尺寸
    ------------------------------------------------------------*/
    void Initialize(int w, int h, double pixelSize);

    /*-----------------------------------------------------------
    ***   Description:计算尺子的每一格代表的长度mm
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ComputeUnitLength();


    /*-----------------------------------------------------------
    ***   Description:计算需要几个间隔
    ***   OUTPUT     :unitToDraw：实际需要的刻度，iSpacePerUnitLength每个刻度里面的小分割
    ***   INPUT      :spaceNum当前应该需要几个刻度
    ------------------------------------------------------------*/
    void CalUnitAndSpace(double spaceNum, int &unitToDraw, int &iSpacePerUnitLength);
                             

    /*-----------------------------------------------------------
    ***   Description:生成点集
    ***   OUTPUT     :
    ***   INPUT      :ptStart，ptEnd开始结束点
    ------------------------------------------------------------*/
    vector<vector<Line>>GeneratePoints(QPointF ptStart, QPointF ptEnd,
        int unitToDraw, 
        const vector<int> &vecRatios, 
        const vector<double> &vecTickLengths);

    /*-----------------------------------------------------------
    ***   Description:更新
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void Update(int w, int h, double pixelSize, vtkSmartPointer<vtkRenderer> spRender);


    /*-----------------------------------------------------------
    ***   Description:计算比例尺的参数
    ***   OUTPUT     : iLength：显示的长度单位像素，iRulerDistance：当前比例尺代表的长度单位mm
    ***   INPUT      :
    ------------------------------------------------------------*/
    void GetParameter(int w, int h, double pixelSize, int &iLength, int &iRulerDistance);


    /*-----------------------------------------------------------
    ***   Description:隐藏
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void Hide();

private:
    //每一大格代表的物理长度，比如10mm
    int m_iUnitLength = 1;
    //每一大格需要到的像素长度，比如300像素
    double m_iPixelNumPerUnit;

    int m_iCellWidth;
    int m_iCellHeight;

    //每个像素所表示的物理长度
    double m_dPixelSize;

    QPointF m_startPoint;
    QPointF m_endPoint;

    vector<vtkSmartPointer<vtkLineSource>> m_vecLineSource;
    vector< vtkSmartPointer<vtkActor>> m_vecLineActor;
    vtkSmartPointer<vtkRenderer>  m_pRender;
    vtkSmartPointer<vtkTextActor> m_pTextActor;
};
#endif // RULER_H_ !   


