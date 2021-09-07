/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            : 
   Author          : 
   Creation Date   : 13. JAN 2021
   Description     : ͼ����ʾ����widget ��QVTKWidget ����
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CELL_H_
#define  CELL_H_
#include "qmainwindow.h"
#include "vtkRenderWindow.h"
#include <qboxlayout.h>
#include "qsharedpointer.h"

#include "vtkImageViewer2.h"
 
#include "vtkStringArray.h"
#include "vtkInteractorStyleTrackball.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCornerAnnotation.h"          
#include <vtkRegularPolygonSource.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkElevationFilter.h>
#include <vtkFollower.h>            

#include "QVTKWidget.h"
#include "ModuleContainer.h"
#include "gespips.h"
#include <vtkCornerAnnotation.h>
#include "Ruler.h"
#include "vtkImageData.h"
#include "vtkAxisActor2D.h"
#include "vtkRenderWindow.h"

#include "commondef.h"
#include "MyVtkAxisActor2D.h"
#include "DataModule.h"


class Cell :public QVTKWidget
{
    Q_OBJECT
public:
    Cell(ContainerPtr spContainer, QWidget *pMainWnd);
    virtual ~Cell();

    //**************************************************
    //	brief   :ֹͣVTK��Ϣѭ�� 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void StopInteractor();               

    //**************************************************
    //	brief   :��λ 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void Location(double ptPos[3]);

    //**************************************************
    //	brief   :���ش�����ʾ����Ϣ����������vtk��interaction�Ĵ��ڴ�С
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void showEvent(QShowEvent *event);

    //**************************************************
    //	brief   :����resizeEvent����radionbuttonλ�� 
    //	in      :QResizeEvent��envet
    //	out     :
    //	return  : 
    //**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;

    /*-----------------------------------------------------------
    ***   Description: ����zoomֵ
    ***   OUTPUT     :
    ***   INPUT      :fZoom����ǰ��zoomֵ
    ------------------------------------------------------------*/
    virtual void UpdateZoom(float fZoom);

    /*-----------------------------------------------------------
    ***   Description: ���±�����
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void UpdateRuler();

    void HideRuler();


    /*-----------------------------------------------------------
    ***   Description: ��ȡ��ǰcell�е�IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    virtual vtkSmartPointer<vtkImageData> GetImageData() = 0;

    /*-----------------------------------------------------------
    ***   Description:��ȡ�ڵ�ǰ��ʾ������ÿ�����ر�ʾ��������
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize() = 0;

		
	/*-----------------------------------------------------------
	***   Description:����dataModule
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetDataModule(DataModule *pDataModule);


	/*-----------------------------------------------------------
	***   Description:�Ƿ����������
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	bool HasCTData();

	bool HasDRData();

 public slots:
    //**************************************************
    //	brief   :��ҳ 
    //	in      :���λ��ǰ����ʾ������λ��
    //	out     :
    //	return  : 
    //**************************************************
    virtual void PageTra(int iIndex);

    //**************************************************
    //	brief   :��ҳ 
    //	in      :ʸ״λ��ҳ
    //	out     :
    //	return  : 
    //**************************************************
    virtual void PageSag(int iIndex);


	/*-----------------------------------------------------------
	***   Description:��ͼ���������ͼ����ʾ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void ClearIamge();

Q_SIGNALS:
	void UpdateScaling(float zoom);

public:
    vtkSmartPointer<vtkRenderWindowInteractor>     m_spInteractor = nullptr;
    ContainerPtr m_spContainer = nullptr;
    vtkSmartPointer< vtkCornerAnnotation>          m_spCornerAnnotationBottomRight;
    vtkSmartPointer<vtkRenderer>  m_pRender;
    vtkSmartPointer<vtkRenderer>  m_pRulerRender;
    vtkSmartPointer<vtkRenderWindow> m_spRenderWindow;

    //��ǰ��ʾ������
    vtkSmartPointer<vtkImageData> m_spDisplayImageData;

    vtkSmartPointer<MyVtkAxisActor2D> m_spAxis;
    Ruler m_ruler;          
    QString m_sCellName;

    OperationAction m_currentAction = OperationAction::ActionPointer;

	DataModule *m_pDataModule = nullptr;
};
#endif // CELL_H_ !   

