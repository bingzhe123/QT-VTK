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
	short oriValue;//ԭʼֵ
	short newValue;//�µ�ֵ
	int index;//�����ݵ�����
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
   //	brief   :��λ 
   //	in      :
   //	out     :
   //	return  : 
   //**************************************************
    virtual void Location(double ptPos[3]);

    void StartInterctor();


    /*-----------------------------------------------------------
    ***   Description:��ɲü�����
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ExecuteClip();


    /*-----------------------------------------------------------
    ***   Description:��ʾ���д���
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/

    void ShowCroppingWnd(double bounds[6], vtkSmartPointer<vtkImageData> spImageData);


    /*-----------------------------------------------------------
    ***   Description: �����ü�����
    ***   OUTPUT     :
    ***   INPUT      : bounds��ǰ�ü�����
    ------------------------------------------------------------*/
    void CreatCroppingData(double bounds[6], vtkSmartPointer<vtkImageData> spImageData);

	/*-----------------------------------------------------------
	***   Description : ���òü�������ɫ
	***   OUTPUT      :
	***   INPUT       : bounds����ǰѡ�������
	------------------------------------------------------------*/
	void SetRegionColor(double bounds[6]);

    /*-----------------------------------------------------------
    ***   Description:�ü���ʱ����굯��ִ��ѡ������
    ***   OUTPUT     :
    ***   INPUT      :bounds����ǰѡ�������
    ------------------------------------------------------------*/
    void SelectRegion(double bounds[6]);
    /*-----------------------------------------------------------
    ***   Description: ��ȡ��ǰ��ͼ�ķ���
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    ViewDirection GetViewDirection();

	/*-----------------------------------------------------------
	***   Description : 3D���Ȳ���
	***   OUTPUT      :
	***   INPUT       : 
	------------------------------------------------------------*/
	void SetMeasure3D(bool bChecked);

	/*-----------------------------------------------------------
	***   Description : ����
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void ScreenCut();

	/*-----------------------------------------------------------
	***   Description : ¼��
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void ScreenCapBtn();

	/*-----------------------------------------------------------
	***   Description:����
	***   OUTPUT     :
	***   INPUT      :startPos��ʼ���꣬endPos�������꣬display����ϵ,dLength����
	------------------------------------------------------------*/
	void OnMeasure(int startPos[2], int endPos[2], double dLength);

	/*-----------------------------------------------------------
	***   Description:action��λ
	***   OUTPUT     :
	***   INPUT      :bHideRuler�Ƿ����س���
	------------------------------------------------------------*/
	void EndMeasue(bool bHideRuler);


	/*-----------------------------------------------------------
	***   Description:���ز����ĳ���
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void HideMeausureRuler();

	/*-----------------------------------------------------------
	***   Description: ����zoomֵ
	***   OUTPUT     :
	***   INPUT      :fZoom����ǰ��zoomֵ
	------------------------------------------------------------*/
	virtual void UpdateZoom(float fZoom);
public slots:
	//**************************************************
	//	brief   :����ͼ 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void TopView();

    //**************************************************
    //	brief   :����ͼ 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void BottomView();

	//**************************************************
	//	brief   :����ͼ 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void LeftView();

	//**************************************************
	//	brief   :����ͼ 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void RightView();

	//**************************************************
	//	brief   :�ָ���ͼ ,ǰ��ͼ
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void ResumeView();

    //**************************************************
    //	brief   :���õ�ǰ��ͼ�����ǲ�ˢ��
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void SetToFrontPos();
    //**************************************************
    //	brief   :����ͼ
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RearView();

	//**************************************************
	//	brief   : α��
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void Color3D();

	//**************************************************
	//	brief   : �ڰ�
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void Gray3D();

	//**************************************************
	//	brief   : ��ɫ
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void NegativeProcess3D();

	//**************************************************
	//	brief   : �ߴ�͸
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThrough3D();

	//**************************************************
	//	brief   : �ߴ�͸
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void LightThrough3D();

	//**************************************************
	//	brief   : ���Ͷ���������
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DecreaseMultiAbsorb();

	//**************************************************
	//	brief   : ���Ӷ���������
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void IncreaseMultiAbsorb();

	//**************************************************
	//	brief   : ��Ե��ǿ
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void EdgeEnhance3D();
    //**************************************************
    //	brief   : �޻����޳�
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RemoveInorg3D();

    //**************************************************
    //	brief   : �л����޳�
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void RemoveOrg3D();

    /*-----------------------------------------------------------
    ***   Description: ��ȡ��ǰcell�е�IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    vtkSmartPointer<vtkImageData> GetImageData();

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

    //**************************************************
//	brief   :����resizeEvent����radionbuttonλ�� 
//	in      :QResizeEvent��envet
//	out     :
//	return  : 
//**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;
    //**************************************************
//	brief   :���ش�����ʾ����Ϣ����������vtk��interaction�Ĵ��ڴ�С
//	in      :
//	out     :
//	return  : 
//**************************************************
    virtual void showEvent(QShowEvent *event);

    /*-----------------------------------------------------------
    ***   Description:��ȡ�ڵ�ǰ��ʾ������ÿ��������Ļ���ж�������
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize();

    /*-----------------------------------------------------------
    ***   Description:����ü���ťִ�к���
    ***   OUTPUT     :
    ***   INPUT      :bCropping:�Ƿ��ǲü�����
    ------------------------------------------------------------*/
    void OnCropping(bool bCropping);

	/*-----------------------------------------------------------
	***   Description:ѡ�������
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SelectSuspiciousObject(QString uid);

	/*-----------------------------------------------------------
	***   Description:ɾ��������
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RemoveSuspiciousObject(QString uid);

	/*-----------------------------------------------------------
	***   Description:��ͼ���������ͼ����ʾ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void ClearIamge();
signals:
		void AddSuspicious(double x, double y, double z, double l, double w, double h, QString uid);
private:

	/*-----------------------------------------------------------
	***   Description:��ʼ��boxwidget
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void InitlizeBoxWidget(std::shared_ptr<BoxWidgetItem> spItem);
		
	/*-----------------------------------------------------------
	***   Description:��bound��worldת��volume����
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void WorldToVolumeBound(vtkSmartPointer<vtkImageData> spImageData, const double worldBound[6], int volumeBound[6]);

	/*-----------------------------------------------------------
	***   Description:�Ѳü�ʱ����Ϊ��ɫ�Ĳ��ֻظ�ΪĬ��ֵ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RecorverToOriginalValue();

		
	/*-----------------------------------------------------------
	***   Description:��ȡ���õ�boxwidget
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	std::shared_ptr<BoxWidgetItem> GetBoxWidgetItem();
	/*-----------------------------------------------------------
	***   Description:��ȡ���е�boxwidget
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
    ***   Description:�ָ�͸����
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    void ResetOpacity();


    /*-----------------------------------------------------------
    ***   Description: �����ά������ļ�ͷ����
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
    int m_iPlaneDirection = 0;//0�����λ�� 1��ʸ״λ
    bool bWindowSizeChanged = true;
    ViewDirection m_currentViewDir = ViewDirection::FrontView;//��ǰ����ͼ����
	int m_iLength = 0;

	vector<UpdatePointList> m_vecUpdateList;//�ü���ʱ���¼ԭʼֵ

	vtkSmartPointer<MyVtkAxisActor2D> m_spMeasureAxis;//ֱ�߲���

    vtkSmartPointer< MyVtkAxisActor3D> m_spAxesActor;
    MyvtkOrientationMarkerWidget * m_pWidget;

};
#endif // CELL_3D_H_ !   

