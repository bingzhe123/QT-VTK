/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 2021.05.30
   Description     : ��Ƭ����
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
    //	brief   :����ͳ����Ϣ 
    //	in      :��ǰѡ��ĵ���Ϣ
    //	out     :
    //	return  : 
    //**************************************************
    void UpdateStatisticInfo(const vector<Point3D> &&vecPoint);
	
	/*-----------------------------------------------------------
	***   Description:���õ�ǰ�Ƿ���ctģʽ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetIsCTMode(bool bIsCT);


	/*-----------------------------------------------------------
	***   Description:����Ӧ����
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void FitWindow();
private slots:
    //**************************************************
    //	brief   : ����ͳ����Ϣ 
    //	in      : ��ǰѡ��ĵ���Ϣ
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
