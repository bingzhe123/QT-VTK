/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CELL_3D_H_
#define  CELL_3D_H_
#include "Cell.h"

#include <share.h>
#include "CustomStyleTrackball.h"
#include "vtkPointPicker.h"
#include "gespips.h"
#include "vtkImagePlaneWidget.h"
#include "vtkVolume.h"
#include "RulerWidget.h"
#include "vtkDistanceWidget.h"
#include "vtkBoxWidget2.h"
#include "vtkBoxRepresentation.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkVolumeProperty.h"
#include "vtkBoxCallback.h"

#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation3D.h>


#include "ScreenCap.h"
#include "MyVtkAxisActor2D.h"

#include "vtkAxesActor.h"
#include "vtkSliderRepresentation2D.h"

#include "MyVtkAxisActor3D.h"
#include "MyvtkOrientationMarkerWidget.h"
struct UpdatePointList
{
	short oriValue;//原始值
	short newValue;//新的值
	int index;//体数据的索引
	UpdatePointList(int _index, short _oriValue, short _newValue) 
		:index(_index), oriValue(_oriValue), newValue(_newValue)
	{

	}
};


class Cell3D : public Cell
{
    Q_OBJECT
public:
    //using Cell::Cell;
	Cell3D(ContainerPtr spContainer, QWidget* pMainWnd, bool bCroppingCell = false);
	

	void LoadData();

    //**************************************************
   //	brief   :定位 
   //	in      :
   //	out     :
   //	return  : 
   //**************************************************
    virtual void Location(double ptPos[3]);

    void StartInterctor();


    /*-----------------------------------------------------------
    ***   Description:完成裁剪操作
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ExecuteClip();


    /*-----------------------------------------------------------
    ***   Description:显示剪切窗口
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/

    void ShowCroppingWnd(double bounds[6], vtkSmartPointer<vtkImageData> spImageData);


    /*-----------------------------------------------------------
    ***   Description: 创建裁剪数据
    ***   OUTPUT     :
    ***   INPUT      : bounds当前裁剪区域
    ------------------------------------------------------------*/
    void CreatCroppingData(double bounds[6], vtkSmartPointer<vtkImageData> spImageData);

	/*-----------------------------------------------------------
	***   Description : 设置裁剪区域颜色
	***   OUTPUT      :
	***   INPUT       : bounds：当前选择的区域
	------------------------------------------------------------*/
	void SetRegionColor(double bounds[6]);

    /*-----------------------------------------------------------
    ***   Description:裁剪的时候鼠标弹起执行选定区域
    ***   OUTPUT     :
    ***   INPUT      :bounds：当前选择的区域
    ------------------------------------------------------------*/
    void SelectRegion(double bounds[6]);
    /*-----------------------------------------------------------
    ***   Description: 获取当前视图的方向
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    ViewDirection GetViewDirection();

	/*-----------------------------------------------------------
	***   Description : 3D长度测量
	***   OUTPUT      :
	***   INPUT       : 
	------------------------------------------------------------*/
	void SetMeasure3D(bool bChecked);

	/*-----------------------------------------------------------
	***   Description : 截屏
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void ScreenCut();

	/*-----------------------------------------------------------
	***   Description : 录屏
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void ScreenCapBtn();

	/*-----------------------------------------------------------
	***   Description:测量
	***   OUTPUT     :
	***   INPUT      :startPos起始坐标，endPos结束坐标，display坐标系,dLength长度
	------------------------------------------------------------*/
	void OnMeasure(int startPos[2], int endPos[2], double dLength);

	/*-----------------------------------------------------------
	***   Description:action复位
	***   OUTPUT     :
	***   INPUT      :bHideRuler是否隐藏尺子
	------------------------------------------------------------*/
	void EndMeasue(bool bHideRuler);


	/*-----------------------------------------------------------
	***   Description:隐藏测量的尺子
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void HideMeausureRuler();

	/*-----------------------------------------------------------
	***   Description: 更新zoom值
	***   OUTPUT     :
	***   INPUT      :fZoom：当前的zoom值
	------------------------------------------------------------*/
	virtual void UpdateZoom(float fZoom);
public slots:
	//**************************************************
	//	brief   :顶视图 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void TopView();

    //**************************************************
    //	brief   :底视图 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void BottomView();

	//**************************************************
	//	brief   :左视图 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void LeftView();

	//**************************************************
	//	brief   :右视图 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void RightView();

	//**************************************************
	//	brief   :恢复视图 ,前视图
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void ResumeView();

    //**************************************************
    //	brief   :设置到前视图但那是不刷新
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void SetToFrontPos();
    //**************************************************
    //	brief   :后视图
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RearView();

	//**************************************************
	//	brief   : 伪彩
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void Color3D();

	//**************************************************
	//	brief   : 黑白
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void Gray3D();

	//**************************************************
	//	brief   : 反色
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void NegativeProcess3D();

	//**************************************************
	//	brief   : 高穿透
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThrough3D();

	//**************************************************
	//	brief   : 高穿透
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void LightThrough3D();

	//**************************************************
	//	brief   : 降低多重吸收率
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DecreaseMultiAbsorb();

	//**************************************************
	//	brief   : 增加多重吸收率
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void IncreaseMultiAbsorb();

	//**************************************************
	//	brief   : 边缘增强
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void EdgeEnhance3D();
    //**************************************************
    //	brief   : 无机物剔除
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RemoveInorg3D();

    //**************************************************
    //	brief   : 有机物剔除
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RemoveOrg3D();

    /*-----------------------------------------------------------
    ***   Description: 获取当前cell中的IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    vtkSmartPointer<vtkImageData> GetImageData();

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

    //**************************************************
//	brief   :重载resizeEvent设置radionbutton位置 
//	in      :QResizeEvent：envet
//	out     :
//	return  : 
//**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;
    //**************************************************
//	brief   :重载窗口显示的消息，用来设置vtk的interaction的窗口大小
//	in      :
//	out     :
//	return  : 
//**************************************************
    virtual void showEvent(QShowEvent *event);

    /*-----------------------------------------------------------
    ***   Description:获取在当前显示窗口下每毫米在屏幕上有多少像素
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize();

    /*-----------------------------------------------------------
    ***   Description:点击裁剪按钮执行函数
    ***   OUTPUT     :
    ***   INPUT      :bCropping:是否是裁剪操作
    ------------------------------------------------------------*/
    void OnCropping(bool bCropping);

	/*-----------------------------------------------------------
	***   Description:选择可疑物
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SelectSuspiciousObject(QString uid);

	/*-----------------------------------------------------------
	***   Description:删除可疑物
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RemoveSuspiciousObject(QString uid);

	/*-----------------------------------------------------------
	***   Description:判图结束后清除图像显示
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void ClearIamge();
signals:
		void AddSuspicious(double x, double y, double z, double l, double w, double h, QString uid);
private:

	/*-----------------------------------------------------------
	***   Description:初始化boxwidget
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void InitlizeBoxWidget(std::shared_ptr<BoxWidgetItem> spItem);
		
	/*-----------------------------------------------------------
	***   Description:把bound从world转向volume坐标
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void WorldToVolumeBound(vtkSmartPointer<vtkImageData> spImageData, const double worldBound[6], int volumeBound[6]);

	/*-----------------------------------------------------------
	***   Description:把裁剪时候标记为红色的部分回复为默认值
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RecorverToOriginalValue();

		
	/*-----------------------------------------------------------
	***   Description:获取可用的boxwidget
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	std::shared_ptr<BoxWidgetItem> GetBoxWidgetItem();
	/*-----------------------------------------------------------
	***   Description:获取所有的boxwidget
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	vector<std::shared_ptr<BoxWidgetItem>> GetAllBoxWidgetItem();


	/*-----------------------------------------------------------
	***   Description:
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void GetMyBound(vtkSmartPointer<vtkImageData> spImageData, double dBound[6]);
	 
    /*-----------------------------------------------------------
    ***   Description:恢复透明度
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ResetOpacity();


    /*-----------------------------------------------------------
    ***   Description: 点击三维坐标轴的箭头函数
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ClickAxis(bool &bX, bool &bY, bool &bZ);
private:
    vtkSmartPointer<CustomStyleTrackballCamera> m_spTrackballCameraStyle;
    vtkSmartPointer<vtkCellPicker>  m_picker;
    vtkSmartPointer<vtkImagePlaneWidget > m_planeWidget;
    vtkSmartPointer< vtkDistanceWidget> m_spDistanceWidget;

	vector<std::shared_ptr<BoxWidgetItem>> m_vecBoxWidgetItem;

  /*  vtkSmartPointer<vtkBoxWidget2> m_spBoxWidget;
    vtkSmartPointer<vtkBoxRepresentation> m_spBoxRep;*/
	vtkSmartPointer<vtkDistanceRepresentation3D> m_spRepresentation;
	vtkSmartPointer<vtkVolume> m_pVolume;
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> m_spVolumeMapper;
    vtkSmartPointer<vtkVolumeProperty> m_spVolumeProperty;
    //vtkSmartPointer<vtkBoxCallback> m_spBoxCallback;
    //vtkSmartPointer<vtkVolume> m_spCroppingVolume;
    //vtkSmartPointer<vtkGPUVolumeRayCastMapper> m_spCroppingMapper;
    //vtkSmartPointer<vtkRenderer>  m_spCroppingRender;
    vtkSmartPointer<vtkSliderRepresentation2D> m_spSliderRep2D;

	ScreenCap m_ScreenCap;
	bool m_bIsCap;
	bool m_bCroppingCell = false;
	bool m_bisEnable;
    int m_imageDims[3];
    int m_iPlaneDirection = 0;//0：横断位， 1，矢状位
    bool bWindowSizeChanged = true;
    ViewDirection m_currentViewDir = ViewDirection::FrontView;//当前的视图方向
	int m_iLength = 0;

	vector<UpdatePointList> m_vecUpdateList;//裁剪的时候记录原始值

	vtkSmartPointer<MyVtkAxisActor2D> m_spMeasureAxis;//直线测量

    vtkSmartPointer< MyVtkAxisActor3D> m_spAxesActor;
    MyvtkOrientationMarkerWidget * m_pWidget;

};
#endif // CELL_3D_H_ !   

