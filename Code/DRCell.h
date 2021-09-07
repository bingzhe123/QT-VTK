/*----------------------------------------------------------------------------
   Component       : DR - View
   Name            :
   Author          :
   Creation Date   : 27. MAY 2021
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  DRCELL_H_
#define  DRCELL_H_
#include "Cell2D.h"
#include <qradiobutton.h>
#include "gespips.h"
#include "vtkTexturedButtonRepresentation2D.h"
#include "vtkImageData.h"
class DRCell : public Cell2D
{
    Q_OBJECT
public:
    DRCell(ContainerPtr spContainer, QWidget *pMainWnd);

    //**************************************************
    //	brief   :button�ĵ���ۺ��� 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void OnChangeDrDisplay();
	//**************************************************
	//	brief   :����showEvent����radionbuttonλ�� 
	//	in      :QShowEvent��envet
	//	out     :
	//	return  : 
	//**************************************************
	virtual void showEvent(QShowEvent *event);


public slots:
	/*-----------------------------------------------------------
	***   Description:��ɫ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void NegativeProcess();
private:

	vtkSmartPointer<vtkTexturedButtonRepresentation2D> m_buttonRepresentation;
	vtkSmartPointer<vtkImageData> m_btnImg1;
	vtkSmartPointer<vtkImageData> m_btnImg2;

	bool m_bDR = true;
};
#endif
