/*----------------------------------------------------------------------------
   Component       : ������
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
    ***   Description:���ò���
    ***   OUTPUT     :
    ***   INPUT      :w,h���ڵĳߴ磬pixelSizeÿ�����ر�ʾ������ߴ�
    ------------------------------------------------------------*/
    void Initialize(int w, int h, double pixelSize);

    /*-----------------------------------------------------------
    ***   Description:������ӵ�ÿһ�����ĳ���mm
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ComputeUnitLength();


    /*-----------------------------------------------------------
    ***   Description:������Ҫ�������
    ***   OUTPUT     :unitToDraw��ʵ����Ҫ�Ŀ̶ȣ�iSpacePerUnitLengthÿ���̶������С�ָ�
    ***   INPUT      :spaceNum��ǰӦ����Ҫ�����̶�
    ------------------------------------------------------------*/
    void CalUnitAndSpace(double spaceNum, int &unitToDraw, int &iSpacePerUnitLength);
                             

    /*-----------------------------------------------------------
    ***   Description:���ɵ㼯
    ***   OUTPUT     :
    ***   INPUT      :ptStart��ptEnd��ʼ������
    ------------------------------------------------------------*/
    vector<vector<Line>>GeneratePoints(QPointF ptStart, QPointF ptEnd,
        int unitToDraw, 
        const vector<int> &vecRatios, 
        const vector<double> &vecTickLengths);

    /*-----------------------------------------------------------
    ***   Description:����
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void Update(int w, int h, double pixelSize, vtkSmartPointer<vtkRenderer> spRender);


    /*-----------------------------------------------------------
    ***   Description:��������ߵĲ���
    ***   OUTPUT     : iLength����ʾ�ĳ��ȵ�λ���أ�iRulerDistance����ǰ�����ߴ���ĳ��ȵ�λmm
    ***   INPUT      :
    ------------------------------------------------------------*/
    void GetParameter(int w, int h, double pixelSize, int &iLength, int &iRulerDistance);


    /*-----------------------------------------------------------
    ***   Description:����
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void Hide();

private:
    //ÿһ������������ȣ�����10mm
    int m_iUnitLength = 1;
    //ÿһ�����Ҫ�������س��ȣ�����300����
    double m_iPixelNumPerUnit;

    int m_iCellWidth;
    int m_iCellHeight;

    //ÿ����������ʾ��������
    double m_dPixelSize;

    QPointF m_startPoint;
    QPointF m_endPoint;

    vector<vtkSmartPointer<vtkLineSource>> m_vecLineSource;
    vector< vtkSmartPointer<vtkActor>> m_vecLineActor;
    vtkSmartPointer<vtkRenderer>  m_pRender;
    vtkSmartPointer<vtkTextActor> m_pTextActor;
};
#endif // RULER_H_ !   


