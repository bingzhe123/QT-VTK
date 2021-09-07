/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            : 
   Author          : 
   Creation Date   : 13. JAN 2021
   Description     : 图像显示窗口widget 从QVTKWidget 派生
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
    //	brief   :停止VTK消息循环 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void StopInteractor();               

    //**************************************************
    //	brief   :定位 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void Location(double ptPos[3]);

    //**************************************************
    //	brief   :重载窗口显示的消息，用来设置vtk的interaction的窗口大小
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void showEvent(QShowEvent *event);

    //**************************************************
    //	brief   :重载resizeEvent设置radionbutton位置 
    //	in      :QResizeEvent：envet
    //	out     :
    //	return  : 
    //**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;

    /*-----------------------------------------------------------
    ***   Description: 更新zoom值
    ***   OUTPUT     :
    ***   INPUT      :fZoom：当前的zoom值
    ------------------------------------------------------------*/
    virtual void UpdateZoom(float fZoom);

    /*-----------------------------------------------------------
    ***   Description: 更新比例尺
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void UpdateRuler();

    void HideRuler();


    /*-----------------------------------------------------------
    ***   Description: 获取当前cell中的IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    virtual vtkSmartPointer<vtkImageData> GetImageData() = 0;

    /*-----------------------------------------------------------
    ***   Description:获取在当前显示窗口下每个像素表示的物理长度
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize() = 0;

		
	/*-----------------------------------------------------------
	***   Description:设置dataModule
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetDataModule(DataModule *pDataModule);


	/*-----------------------------------------------------------
	***   Description:是否加载了数据
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	bool HasCTData();

	bool HasDRData();

 public slots:
    //**************************************************
    //	brief   :翻页 
    //	in      :横断位当前的显示的索引位置
    //	out     :
    //	return  : 
    //**************************************************
    virtual void PageTra(int iIndex);

    //**************************************************
    //	brief   :翻页 
    //	in      :矢状位翻页
    //	out     :
    //	return  : 
    //**************************************************
    virtual void PageSag(int iIndex);


	/*-----------------------------------------------------------
	***   Description:判图结束后清除图像显示
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

    //当前显示的数据
    vtkSmartPointer<vtkImageData> m_spDisplayImageData;

    vtkSmartPointer<MyVtkAxisActor2D> m_spAxis;
    Ruler m_ruler;          
    QString m_sCellName;

    OperationAction m_currentAction = OperationAction::ActionPointer;

	DataModule *m_pDataModule = nullptr;
};
#endif // CELL_H_ !   

