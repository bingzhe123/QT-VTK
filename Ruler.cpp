#include "Ruler.h"
#include <qvector.h>
#include "vtkDataSetMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkCoordinate.h"
#include "vtkTextProperty.h"


Ruler::Ruler()
{
    m_startPoint.setX(0.05);
    m_startPoint.setY(0.01);
    m_endPoint.setX(0.95);
    m_endPoint.setY(0.01);
}

Ruler::~Ruler()
{
}
/*-----------------------------------------------------------
***   Description:
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Ruler::Initialize(int w, int h, double pixelSize)
{
    m_iCellWidth = w;
    m_iCellHeight = h;
    m_dPixelSize = pixelSize;
}

/*-----------------------------------------------------------
***   Description:计算尺子的每一格代表的长度mm
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Ruler::ComputeUnitLength()
{
    if (m_iUnitLength < 1)
    {
        m_iUnitLength = 1;
        m_iPixelNumPerUnit = m_iUnitLength / m_dPixelSize;
        return;
    }
    m_iPixelNumPerUnit = m_iUnitLength / m_dPixelSize;
    if (m_iPixelNumPerUnit < 1e-5)
    {
        m_iUnitLength = 1;
        return;
    }
    auto dStartx = m_iCellWidth * m_startPoint.x();
    auto dEndx = m_iCellWidth * m_endPoint.x();
    auto length = fabs(dEndx - dStartx);
    auto spaceNum = length / m_iPixelNumPerUnit;
    if (spaceNum > 10)
    {
        m_iUnitLength *= 10;
        ComputeUnitLength();
    }
    if (spaceNum < 1)
    {
        m_iUnitLength /= 10;
        ComputeUnitLength();
    }
}

/*-----------------------------------------------------------
***   Description:计算需要几个间隔
***   OUTPUT     :unitToDraw：实际需要的刻度，iSpacePerUnitLength每个刻度里面的小分割
***   INPUT      :spaceNum当前应该需要几个刻度
------------------------------------------------------------*/
void Ruler::CalUnitAndSpace(double dSpaceNum, int &unitToDraw, int &iSpacePerUnitLength)
{
    if (dSpaceNum > 1e-6 && dSpaceNum < 2)
    {                                     
        unitToDraw = 1;
    }
    else if (dSpaceNum < 3)
    {
        unitToDraw = 2;
    }
    else if (dSpaceNum < 5)
    {
        unitToDraw = 3;
        iSpacePerUnitLength = 5;
    }
    else
    {
        unitToDraw = 5;
        iSpacePerUnitLength = 5;
    }
}

/*-----------------------------------------------------------
  ***   Description:生成点集
  ***   OUTPUT     :
  ***   INPUT      :ptStart，ptEnd开始结束点
  ------------------------------------------------------------*/
vector<vector<Line>>Ruler::GeneratePoints(QPointF ptStart, QPointF ptEnd,
    int unitToDraw,
    const vector<int> &vecRatios,
    const vector<double> &vecTickLengths)
{
    vector<vector<Line>> lineGroup;
    int iCount = vecRatios.size();
    for (int i = 0; i < iCount; i++)
    {
        int tempNum = unitToDraw * vecRatios[i];
        int tempNumInt = tempNum;

        vector<Line> vecLine;
        auto pTempStart = ptStart;
        auto dDelat = (ptEnd.x() - ptStart.x()) / tempNumInt;
        for (int  num = 0; num <= tempNumInt; num++)
        {
            Line line;
            line.ptStart = pTempStart;
            line.ptEnd = pTempStart;
            line.ptEnd.setY(pTempStart.y() - vecTickLengths[i]);
            pTempStart.setX(pTempStart.x()+ dDelat);
            vecLine.push_back(line);
        }
        vector<Line> vecTempLine;
        for (int j = 0; j < vecLine.size(); j++)
        {
            if (i == 0)
            {
                vecTempLine = vecLine;
                break;
            }
            for (int k = 1; k <= i; k++)
            {
                if (j%vecRatios[k] == 0)
                { 
                    continue;
                }
                vecTempLine.push_back(vecLine[j]);
            }  
        }
        lineGroup.push_back(vecTempLine);
    }
    vector<Line> vecXaxis; //比例尺长轴
    Line line;
    line.ptStart = ptStart;
    line.ptEnd = ptEnd;
    vecXaxis.push_back(line);
    lineGroup.push_back(vecXaxis);
    return std::move(lineGroup);
}


void Ruler::GetParameter(int w, int h, double pixelSize, int &iLengthToDraw, int &iRulerDistance)
{
    m_iCellWidth = w;
    m_iCellHeight = h;
    m_dPixelSize = pixelSize;
    ComputeUnitLength();
    int iSpacePerUnitLength = 10; //每个大刻度中的小刻度
    auto dStartx = m_iCellWidth * m_startPoint.x();
    auto dStarty = m_iCellHeight;
    auto dEndx = m_iCellWidth * m_endPoint.x();
    auto dEndy = m_iCellHeight;
    auto length = fabs(dEndx - dStartx);
    double dSpaceNum = length / m_iPixelNumPerUnit;
    int iUnitToDraw = 0;
    CalUnitAndSpace(dSpaceNum, iUnitToDraw, iSpacePerUnitLength);
    iLengthToDraw = iUnitToDraw * m_iPixelNumPerUnit;
    iRulerDistance = iUnitToDraw*m_iUnitLength;
}

/*-----------------------------------------------------------
***   Description:生成所有刻度需要的点
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Ruler::Update(int w, int h, double pixelSize, vtkSmartPointer<vtkRenderer> spRender)
{
    m_iCellWidth = w;
    m_iCellHeight = h;
    m_dPixelSize = pixelSize;
    m_pRender = spRender;
    ComputeUnitLength();
    int iSpacePerUnitLength = 10; //每个大刻度中的小刻度
    auto dStartx = m_iCellWidth * m_startPoint.x();
    auto dStarty = m_iCellHeight ;
    auto dEndx = m_iCellWidth * m_endPoint.x();
    auto dEndy = m_iCellHeight;
    auto length = fabs(dEndx - dStartx);
    double dSpaceNum = length / m_iPixelNumPerUnit;
    int iUnitToDraw = 0;
    CalUnitAndSpace(dSpaceNum, iUnitToDraw, iSpacePerUnitLength);
    vector<int> vecRatios = { 1, iSpacePerUnitLength };

    double iLengthToDraw = iUnitToDraw * m_iPixelNumPerUnit;
    auto ptStart = QPointF(dStartx, dStarty);
    auto ptEnd = QPointF(dEndx, dEndy);
    ptStart.setX((m_iCellWidth - iLengthToDraw) / 2);
    ptEnd.setX((m_iCellWidth + iLengthToDraw) / 2);

    double dMainRatio = 0.02;
    double dSubRatio = 0.012;
    double dMainTickLength = (m_iCellWidth + m_iCellHeight)* dMainRatio;
    if (dMainTickLength < 2)
    {
        dMainTickLength = 2;
    }
    double dSubTickLength = (m_iCellWidth + m_iCellHeight)* dSubRatio;
    if (dSubTickLength < 1)
    {
        dSubTickLength = 1;
    }
    vector<double> vecTickLengths;
    vecTickLengths.push_back(dMainTickLength);
    vecTickLengths.push_back(dSubTickLength);
    auto vecLineGroup = GeneratePoints(ptStart, ptEnd, iUnitToDraw, vecRatios, vecTickLengths);

    for (auto  actor: m_vecLineActor)
    {
        actor->SetVisibility(false);
    }

    vtkSmartPointer<vtkCoordinate> c = vtkCoordinate::New();
    c->SetCoordinateSystemToDisplay();
    

    int i = 0;
    int iCurrentSize = m_vecLineActor.size();
    for (auto &vecLine : vecLineGroup)
    {
        for (auto &line:vecLine )
        {

            double _p1[3] = { line.ptStart.x(), line.ptStart.y(), 0.0 };
            double _p2[3] = { line.ptEnd.x(), line.ptEnd.y(), 0.0 };
   
            c->SetValue(_p1[0], _p1[1], _p1[2]);
            auto res = c->GetComputedWorldValue(m_pRender);
            double p1[3] = { res[0],res[1],res[2] };
       
            c->SetValue(_p2[0], _p2[1], _p2[2]);
            res = c->GetComputedWorldValue(m_pRender);
            double p2[3] = { res[0],res[1],res[2] };


            if (i<iCurrentSize)
            {
                auto spLineSource = m_vecLineSource[i];
                spLineSource->SetPoint1(p1);
                spLineSource->SetPoint2(p2);
                spLineSource->Update();
                m_vecLineActor[i]->SetVisibility(true);
                i++;
            }
            else
            {
                vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
                lineSource->SetPoint1(p1);
                lineSource->SetPoint2(p2);
                lineSource->Update();
                m_vecLineSource.push_back(lineSource);
                auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
                mapper->SetInputConnection(lineSource->GetOutputPort());

                vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
                lineActor->SetMapper(mapper);
                lineActor->GetProperty()->SetColor(1, 0, 0);
                lineActor->GetProperty()->SetLineWidth(3);
                m_pRender->AddViewProp(lineActor);
                m_vecLineActor.push_back(lineActor);
            }
        }
    }

    if (m_pTextActor == nullptr)
    {
        m_pTextActor = vtkSmartPointer<vtkTextActor>::New();
        //m_pTextActor->SetTextScaleModeToProp();
        m_pTextActor->GetTextProperty()->SetFontSize(18);
        //m_pTextActor->GetTextProperty()->SetFontFamilyToArial();
        //m_pTextActor->GetTextProperty()->SetJustificationToCentered();
        //m_pTextActor->GetTextProperty()->BoldOn();
        //m_pTextActor->GetTextProperty()->ItalicOn();
        //m_pTextActor->GetTextProperty()->ShadowOn();
        m_pTextActor->GetTextProperty()->SetColor(1, 0, 0);
        m_pRender->AddViewProp(m_pTextActor);
    }
    m_pTextActor->SetDisplayPosition(ptEnd.x(), ptEnd.y()-20);//Position
    QString sText;
    sText = QString::number(iUnitToDraw*m_iUnitLength, 'f', 2);
    sText += "mm";

    m_pTextActor->SetInput(sText.toStdString().c_str());

    m_pRender->GetRenderWindow()->Render();
    //m_pRender->Render();
}

/*-----------------------------------------------------------
***   Description:隐藏
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Ruler::Hide()
{               
    for (auto actor : m_vecLineActor)
    {
        actor->SetVisibility(false);
    }
}