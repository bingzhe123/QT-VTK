/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 2021.05.30
   Description     : 切片窗口
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  SLICECELL_H_
#define  SLICECELL_H_

#include "Cell2D.h"
#include <qradiobutton.h>
#include "DisplayPickerValueWidget.h"
#include <qlabel.h>
#include <qvector.h>
#include <qevent.h>
#include <commondef.h>

class SliceCell : public Cell2D
{
    Q_OBJECT

public:
    SliceCell(ContainerPtr spContainer, QWidget *pMainWnd);
    virtual ~SliceCell();
    virtual void Initialize();

    //**************************************************
    //	brief   :更新统计信息 
    //	in      :当前选择的点信息
    //	out     :
    //	return  : 
    //**************************************************
    void UpdateStatisticInfo(const vector<Point3D> &&vecPoint);
	
	/*-----------------------------------------------------------
	***   Description:设置当前是否是ct模式
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetIsCTMode(bool bIsCT);


	/*-----------------------------------------------------------
	***   Description:自适应窗口
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void FitWindow();
private slots:
    //**************************************************
    //	brief   : 更新统计信息 
    //	in      : 当前选择的点信息
    //	out     :
    //	return  : 
    //**************************************************
    void Location(QVector<int> vecPos);

private:
    vtkSmartPointer<vtkCellPicker>  m_picker;
    QRadioButton *m_pRadioButtonChangeDRView;
    QLabel *m_pLabel;
	bool m_bCTMode = false;
};

#endif
