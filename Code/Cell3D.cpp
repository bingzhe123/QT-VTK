#include "Cell3D.h"
#include "cell2d.h"
#include "QDateTime.h"
#include "QScreen.h"
#include "QDir.h"

#include "vtkAutoInit.h"
#include "IPSImportImagesData.h"
#include "IPSVolumeRendering.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
#include "vtkRenderWindow.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageData.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTesting.h"
#include "vtkImageCast.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkRendererCollection.h"
#include "CustomStyleTrackball.h"
#include "vtkProperty.h"
#include "DataModule.h"
#include "vtkCamera.h"
#include <vtkAxisActor2D.h>
#include "commondef.h"
#include "MainViewWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkLineSource.h"
#include "vtkDataSetMapper.h"
#include "vtkTransform.h"
#include "vtkPlanes.h"
#include "vtkCallbackCommand.h"
#include "vtkBoxCallback.h"
#include "vtkAlgorithmOutput.h"
#include "vtkInformation.h"
#include "vtkImageCanvasSource2D.h"
#include  "vtkProperty2D.h"
#include <QUuid>
#include <QMessageBox.h>
#include "vtkCaptionActor2D.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkPropCollection.h"
#include "vtkLineRepresentation.h"
#include "vtkLineWidget2.h"
#include "vtkSliderWidget.h"
#include "vtkSliderRepresentation2D.h"
#include "vtkSliderCallBack.h"
#include "MyvtkOrientationMarkerWidget.h"
vtkStandardNewMacro(CustomStyleTrackballCamera);

extern QString ScreenShotPath;
const float g_fExtent = 1.5;
const float g_fPlaneExtent = 1.0;

extern bool checkStatus;

 

Cell3D::Cell3D(ContainerPtr spContainer, QWidget* pMainWnd, bool bCroppingCell)
:Cell(spContainer, pMainWnd)
{
    m_sCellName = "Cell3D";
	m_bCroppingCell = bCroppingCell;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Cell3D::Cell3D");

	m_spDistanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
	m_spRepresentation = vtkSmartPointer<vtkDistanceRepresentation3D>::New();
	m_bisEnable = false;

	m_bIsCap = false;
    auto pRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_spRenderWindow = pRenderWindow;
    //pRenderWindow->SetSize(300, 100);
    this->SetRenderWindow(pRenderWindow);
    //auto pRenderWindow = this->GetRenderWindow();
    pRenderWindow->SetInteractor(m_spInteractor);
    m_spTrackballCameraStyle = vtkSmartPointer<CustomStyleTrackballCamera>::New();
    m_spInteractor->SetRenderWindow(pRenderWindow);
    m_spInteractor->SetInteractorStyle(m_spTrackballCameraStyle);
    m_spTrackballCameraStyle->SetInteractor(m_spInteractor);
    m_spTrackballCameraStyle->m_updateZoomFun = [this](float fZoom)
    {
        this->UpdateZoom(fZoom);
    };
    m_pRender = vtkSmartPointer<vtkRenderer>::New();
    m_pRender->SetBackground(255, 255, 255);//设置背景色
    //m_spCroppingRender = vtkSmartPointer<vtkRenderer>::New();
    //m_spCroppingRender->SetBackground(128, 128, 128);//设置背景色
    //m_spCroppingRender->SetInteractive(true);

    pRenderWindow->AddRenderer(m_pRender);
   // m_pRender->SetViewport(0, 0, 1, 0.95);
                       
    m_spTrackballCameraStyle->SetCurrentRenderer(m_pRender);
    m_spTrackballCameraStyle->SetDefaultRenderer(m_pRender);
    m_spTrackballCameraStyle->m_pCurrentCell = this;
    m_picker = vtkSmartPointer<vtkCellPicker>::New();
    m_picker->SetTolerance(0.005);
	m_picker->SetVolumeOpacityIsovalue(-1);
    m_spInteractor->SetPicker(m_picker);
    m_spInteractor->Initialize();

    m_pVolume = vtkSmartPointer<vtkVolume>::New();
    m_spTrackballCameraStyle->m_pVolume = m_pVolume;
   // m_spCroppingVolume = vtkSmartPointer<vtkVolume>::New();
    m_spVolumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
   // m_spCroppingMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    /*m_spCroppingMapper->SetCropping(true);
    m_spCroppingMapper->SetCroppingRegionFlagsToCross();*/


    ResetOpacity();

    m_planeWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
    m_planeWidget->SetInteractor(m_spInteractor);
    m_planeWidget->GetPlaneProperty()->SetColor(1, .0, .0);
    m_planeWidget->GetPlaneProperty()->SetLineWidth(1.5);
    m_planeWidget->SetDefaultRenderer(m_pRender);
    m_planeWidget->SetResliceInterpolateToLinear();
    m_planeWidget->UserControlledLookupTableOff();
    m_planeWidget->SetTextureVisibility(false);
    //m_planeWidget->InteractionOff();
    //m_planeWidget->TextureInterpolateOn();
    //m_planeWidget->RestrictPlaneToVolumeOn();

    m_pRender->AddViewProp(m_spCornerAnnotationBottomRight);

	m_spDistanceWidget->SetInteractor(m_spInteractor);
	m_spDistanceWidget->SetRepresentation(m_spRepresentation);
	m_spDistanceWidget->SetPriority(0.9);
	static_cast<vtkDistanceRepresentation*> (m_spDistanceWidget->GetRepresentation())->SetLabelFormat("%-#6.2f mm");
	m_spDistanceWidget->ManagesCursorOn();
	//m_spDistanceWidget->On();
//    m_spBoxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
//    //m_spBoxWidget->GetFaceProperty()->SetColor(0.6, 0.6, 0.2);
//    //m_spBoxWidget->GetFaceProperty()->SetOpacity(0.25);
//    m_spBoxWidget->SetInteractor(m_spInteractor);    
//    m_spBoxWidget->SetDefaultRenderer(m_pRender);
//
//    m_spBoxRep = vtkSmartPointer<vtkBoxRepresentation>::New();
//    m_spBoxRep->SetPlaceFactor(1);
//	m_spBoxRep->HandlesOff();
//	m_spBoxRep->GetOutlineProperty()->SetColor(1,  0, 0);
//	m_spBoxRep->GetOutlineProperty()->SetLineWidth(5);
//	m_spBoxRep->GetSelectedOutlineProperty()->SetEdgeVisibility(false);
//	m_spBoxRep->GetSelectedHandleProperty()->SetEdgeVisibility(false);
//	m_spBoxRep->OutlineCursorWiresOff();
//	//m_spBoxRep->GetSelectedFaceProperty()->SetEdgeVisibility(false);
//    auto state = m_spBoxRep->GetInteractionState();
//    m_spBoxWidget->SetRepresentation(m_spBoxRep);
//    m_spBoxWidget->RotationEnabledOff();
//
///*    auto interactionCallback = vtkSmartPointer<vtkCallbackCommand>::New();
//    interactionCallback->SetCallback(BoundBoxCallBack);
//    m_spBoxWidget->AddObserver(vtkCommand::InteractionEvent, interactionCallback)*/;
//    m_spBoxCallback = vtkSmartPointer<vtkBoxCallback>::New();
//    m_spBoxCallback->m_spVolumeMapper = m_spVolumeMapper;
//    m_spBoxWidget->AddObserver(vtkCommand::EndInteractionEvent, m_spBoxCallback);

    if (bCroppingCell)
    {
        //m_spVolumeMapper->SetCropping(true);
        //m_spVolumeMapper->SetCroppingRegionFlagsToCross();
        m_spVolumeMapper->SetCroppingRegionFlagsToSubVolume();
       // m_spVolumeMapper->SetCroppingRegionFlagsToFence();
        m_pRender->SetBackground(255, 255, 255);//设置背景色
	}
    else
    {
        m_spVolumeMapper->SetCropping(true);
        m_spVolumeMapper->SetCroppingRegionFlagsToSubVolume();
		m_planeWidget->InteractionOff();
		m_planeWidget->SetPlaceFactor(g_fPlaneExtent);
        //m_spVolumeMapper->SetCroppingRegionFlagsToCross();
    }
	m_spAxis->SetVisibility(false);
	m_pVolume->SetMapper(m_spVolumeMapper);
    m_pVolume->SetProperty(m_spVolumeProperty);
    m_pRender->AddVolume(m_pVolume); 
    m_pRender->AddActor2D(m_spAxis);
	m_spMeasureAxis = vtkSmartPointer<MyVtkAxisActor2D>::New();
	m_spMeasureAxis->SetTickLength(20);
	m_spMeasureAxis->SetTickOffset(10);
	m_spMeasureAxis->SetTitle("mm");
	m_spMeasureAxis->SetMinorTickLength(9);
	m_spMeasureAxis->SetTitlePosition(1);
	m_spMeasureAxis->SetLabelFormat("%0.1f");
	m_spMeasureAxis->GetProperty()->SetColor(0, 0, 1);
	m_spMeasureAxis->GetProperty()->SetLineWidth(3);
	m_spMeasureAxis->VisibilityOff();
	m_spMeasureAxis->RulerModeOn();
	m_spMeasureAxis->AdjustLabelsOff();
	m_spMeasureAxis->TitleVisibilityOff();

	m_pRender->AddActor2D(m_spMeasureAxis);

    m_spAxesActor = vtkSmartPointer<MyVtkAxisActor3D>::New();
    m_spAxesActor->SetXAxisLabelText("x");
    m_spAxesActor->SetYAxisLabelText("y");
    m_spAxesActor->SetZAxisLabelText("z");
    double dLength = 100;
    double dLengh[3] = { dLength,dLength,dLength };
    m_spAxesActor->SetTotalLength(dLengh);
   /* m_spAxesActor->SetShaftType(0);
    m_spAxesActor->SetCylinderRadius(0.02);*/

    //m_spAxesActor->SetNormalizedShaftLength(1, 1, 1);
   // m_spAxesActor->SetNormalizedTipLength(0.1, 0.1, 0.1);
    //m_spAxesActor->SetSphereRadius(0.5);
   // m_spAxesActor->SetTipTypeToSphere();
   // m_spAxesActor->GetXAxisTipProperty()->SetPointSize(0.1);
   // m_spAxesActor->SetShaftTypeToCylinder();
  //  m_spAxesActor->SetVisibility(true);

   // 修改vtkAxesActor默认的字体颜色，Axes为vtkAxesActor的对象指针
    //auto spProperty = m_spAxesActor->GetXAxisCaptionActor2D()->GetProperty();
    //m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 0);//修改X字体颜色为红色
    //m_spAxesActor->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    //m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    //m_spAxesActor->GetXAxisShaftProperty()->SetColor(1, 0, 0);
    //m_spAxesActor->GetXAxisTipProperty()->SetColor(1, 0, 0);
    //m_spAxesActor->GetXAxisTipProperty()->SetPointSize(0.1);
    //m_spAxesActor->SetNormalizedShaftLength(1, 1, 1);
    //m_spAxesActor->GetXAxisCaptionActor2D()->BorderOff();

    double dTipLength = 0.2;//箭头的大小
    m_spAxesActor->SetNormalizedTipLength(dTipLength, dTipLength, dTipLength);    
    //m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    //m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(1);
    //m_spAxesActor->GetYAxisCaptionActor2D()->GetProperty()->SetColor(0, 2, 0);//修改Y字体颜色为绿色
    //m_spAxesActor->GetZAxisCaptionActor2D()->GetProperty()->SetColor(0, 0, 3);//修改Z字体颜色为蓝色   
    /*spProperty->SetPointSize(1);
    m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(100);
    m_spAxesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0.5, 0.5, 0.5);
    m_spAxesActor->SetNormalizedTipLength(0.1, 0.1, 0.1);*/
    m_spAxesActor->SetPosition(0, 0, 0);
    m_spAxesActor->SetOrigin(0.5, 0.5, 0);
    m_spAxesActor->PickableOn();
 
    m_pWidget = MyvtkOrientationMarkerWidget::New();
    m_pWidget->m_XAxisTip = m_spAxesActor->GetXAxisActor();
    m_pWidget->m_YAxisTip = m_spAxesActor->GetYAxisActor();
    m_pWidget->m_ZAxisTip = m_spAxesActor->GetZAxisActor();
    m_pWidget->m_pClickTipFunc = [this](bool &bX, bool &bY, bool &bZ)
    {
        ClickAxis(bX, bY, bZ);
    };
    m_pWidget->SetDefaultRenderer(m_pRender); 
    m_pWidget->SetCurrentRenderer(m_pRender);
    m_pWidget->SetOrientationMarker(m_spAxesActor);
    m_pWidget->SetInteractor(m_spInteractor);
    m_pWidget->SetViewport(0, 0, 0.25, 0.25); //设置相机的位置
    m_pWidget->SetEnabled(true);
    m_pWidget->SetOutlineColor(1, 0, 0);
    m_pWidget->PickingManagedOn();
    m_pWidget->InteractiveOn();
    m_spAxesActor->SetVisibility(false);

   //实例化vtkSliderRepresentation3D，并设置属性。该对象用做滑块在场景中的3D表示
   m_spSliderRep2D = vtkSmartPointer<vtkSliderRepresentation2D>::New();
   m_spSliderRep2D = vtkSmartPointer<vtkSliderRepresentation2D>::New();
   m_spSliderRep2D->SetMinimumValue(0);
   m_spSliderRep2D->SetMaximumValue(100);
   m_spSliderRep2D->SetValue(30); // 调整Sphere的Theta
   //pSliderRep2D->SetTitleText("Sphere Resolution");                     
   m_spSliderRep2D->GetSliderProperty()->SetColor(1, 0, 0);

   m_spSliderRep2D->GetSliderProperty()->SetColor(1, 0, 0);//red
   m_spSliderRep2D->GetTitleProperty()->SetColor(1, 0, 0);//red
   m_spSliderRep2D->GetLabelProperty()->SetColor(1, 1, 0);//red
   m_spSliderRep2D->SetLabelFormat("%0.1f");
   m_spSliderRep2D->GetSelectedProperty()->SetColor(0, 0, 1);//green
   m_spSliderRep2D->GetTubeProperty()->SetColor(0, 1, 0);//yellow
   m_spSliderRep2D->GetCapProperty()->SetColor(0, 0, 0);//yellow
  
   vtkSliderWidget*sliderWidget = vtkSliderWidget::New();
   sliderWidget->KeyPressActivationOff();
   sliderWidget->SetInteractor(m_spInteractor);
   sliderWidget->SetRepresentation(m_spSliderRep2D);
   sliderWidget->SetAnimationModeToAnimate();
   sliderWidget->EnabledOn();//用于使用滑块

   vtkSmartPointer<vtkSliderCallBack> callback = vtkSmartPointer<vtkSliderCallBack>::New();
   sliderWidget->AddObserver(vtkCommand::InteractionEvent, callback);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell3D::Cell3D");
}


void Cell3D::InitlizeBoxWidget(std::shared_ptr<BoxWidgetItem> spItem)
{

	spItem->m_spBoxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
	//m_spBoxWidget->GetFaceProperty()->SetColor(0.6, 0.6, 0.2);
	//m_spBoxWidget->GetFaceProperty()->SetOpacity(0.25);
	spItem->m_spBoxWidget->SetInteractor(m_spInteractor);
	spItem->m_spBoxWidget->SetDefaultRenderer(m_pRender);
	spItem->m_spBoxRep = vtkSmartPointer<vtkBoxRepresentation>::New();
	spItem->m_spBoxRep->SetPlaceFactor(1);
	spItem->m_spBoxRep->HandlesOff();
	spItem->m_spBoxRep->GetOutlineProperty()->SetColor(1, 0, 0);
	spItem->m_spBoxRep->GetOutlineProperty()->SetLineWidth(5);
	spItem->m_spBoxRep->GetSelectedOutlineProperty()->SetEdgeVisibility(false);
	spItem->m_spBoxRep->GetSelectedHandleProperty()->SetEdgeVisibility(false);
	spItem->m_spBoxRep->OutlineCursorWiresOff();
	//m_spBoxRep->GetSelectedFaceProperty()->SetEdgeVisibility(false);
	spItem->m_spBoxWidget->SetRepresentation(spItem->m_spBoxRep);
	spItem->m_spBoxWidget->RotationEnabledOff();

	/*    auto interactionCallback = vtkSmartPointer<vtkCallbackCommand>::New();
		interactionCallback->SetCallback(BoundBoxCallBack);
		m_spBoxWidget->AddObserver(vtkCommand::InteractionEvent, interactionCallback)*/;
	spItem->m_spBoxCallback = vtkSmartPointer<vtkBoxCallback>::New();
	spItem->m_spBoxCallback->m_spVolumeMapper = m_spVolumeMapper;
	spItem->m_spBoxWidget->AddObserver(vtkCommand::EndInteractionEvent, spItem->m_spBoxCallback);
	spItem->m_uid = QUuid::createUuid().toString();
}
void Cell3D::LoadData()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell3D::Render");
	m_planeWidget->On();
	m_planeWidget->InteractionOff();
	m_spAxis->SetVisibility(true);

    auto spImageData = m_pDataModule->GetOriginalImageData();
	m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
	m_spDisplayImageData->DeepCopy(spImageData);
    m_spDisplayImageData->UpdateCellGhostArrayCache();
    m_spDisplayImageData->UpdatePointGhostArrayCache();
    spImageData->GetDimensions(m_imageDims);  
    double dSpace[3];
    spImageData->GetSpacing(dSpace);
    m_spVolumeMapper->SetInputData(m_spDisplayImageData);
    
    double bounds[6];
    m_pVolume->GetBounds(bounds);
    m_pVolume->SetOrigin(0.5,0.5,0.5);

    m_spTrackballCameraStyle->m_spImageData = m_spDisplayImageData;
	GetMyBound(m_spDisplayImageData, bounds);
    memcpy(m_spTrackballCameraStyle->m_defualtBounds, bounds, sizeof(double)*6);
    //volume->GetVolumes(vktPropCollection *vc);
    // Render
    auto m_renWin = this->GetRenderWindow();
    m_renWin->SetMultiSamples(0);
  
    int valid = m_spVolumeMapper->IsRenderSupported(m_renWin, m_spVolumeProperty);
    if (!valid)
    {
        cout << "Required extensions not supported." << endl;
        return;
    }

    m_pRender->AddVolume(m_pVolume);
   
    ResumeView();
    m_renWin->Render();
    m_planeWidget->SetPlaneOrientationToZAxes();
    double dBounds[6];
    m_pVolume->GetBounds(dBounds);
    m_planeWidget->PlaceWidget(dBounds);
    m_spVolumeMapper->SetCroppingRegionPlanes(m_pVolume->GetBounds());
    //FitWindow();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell3D::Render");

}

void Cell3D::Location(double ptPos[3])
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell3D::Location");
    auto pSliceCell = m_spContainer->GetWidget<Cell2D>(ksSliceCellWidget);
    int iIndex = (int)ptPos[2];
    pSliceCell->PageTra(iIndex);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell3D::Location");
}
void Cell3D::StartInterctor()
{

}

/*-----------------------------------------------------------
***   Description:执行裁剪操作
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Cell3D::ExecuteClip()
{  
	//RecorverToOriginalValue();
	
	if (m_bCroppingCell)
	{
		auto pCell = m_spContainer->GetWidget<Cell3D>(ks3DCellWidget);
		std::shared_ptr<BoxWidgetItem> pItem = nullptr;
		for each (auto item in pCell->GetAllBoxWidgetItem())
		{
			if (item->m_isSelect && item->m_isUsed)
			{
				pItem = item;
				break;
			}
		} 
		if (pItem != nullptr)
		{
			double bounds[6];
			pItem->m_spBoxCallback->GetCurrentBounds(bounds);
			pCell->SetRegionColor(bounds);
		}
		
		this->hide();
		pCell->show();
		return;
	}

	//m_spBoxCallback->GetCurrentBounds(bounds);
	std::shared_ptr<BoxWidgetItem> pItem = nullptr;
	for each (auto item in m_vecBoxWidgetItem)
	{
		if (item->m_isSelect && item->m_isUsed)
		{
			pItem = item;
			break;
		}
	}
	if (pItem == nullptr)
	{
		return;
	}
	double bounds[6];
	pItem->m_spBoxCallback->GetCurrentBounds(bounds);
	for (size_t i = 0; i < 6; i++)
	{
		if (bounds[i] < 0)
		{
			bounds[i] = 0;
		}
	}
    auto pCroppingCell = m_spContainer->GetWidget<Cell3D>(ksCroppintCellWidget);
	RecorverToOriginalValue();
    pCroppingCell->ShowCroppingWnd(bounds, m_spDisplayImageData);
	this->hide();
#pragma region 添加可疑物
	//emit AddSuspicious(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5], "" );

#pragma endregion
	//todo
   /* auto spDataModule = m_spContainer->GetModule<DataModule>(ksDataModule);
    auto spImageData = spDataModule->GetOriginalImageData();
    m_spDisplayImageData = spImageData;
    auto boundOri = m_spDisplayImageData->GetBounds();
    m_spBoxRep->PlaceWidget(boundOri);
    memcpy(m_spBoxCallback->m_currentBounds, boundOri, sizeof(double) * 6);
    m_spVolumeMapper->SetCroppingRegionPlanes(boundOri);
    m_spVolumeMapper->SetInputData(m_spDisplayImageData.GetPointer());
    m_spBoxWidget->Off();
	m_pRender->RemoveVolume(m_pVolume);
	m_pRender->AddVolume(m_pVolume);
    ResumeView();*/
}

void Cell3D::ShowCroppingWnd(double bound[6], vtkSmartPointer<vtkImageData> spImageData)
{
    CreatCroppingData(bound, spImageData);
    m_pRender->ResetCamera();
    m_pRender->ResetCameraClippingRange();
    double propBounds[6];
    m_pRender->ComputeVisiblePropBounds();
    this->show();
    ResumeView();
}

void Cell3D::CreatCroppingData(double bound[6], vtkSmartPointer<vtkImageData> spImageData)
{ 
    int dim[3];
    double spacing[3], origin[3];
    spImageData->GetSpacing(spacing);
    spImageData->GetOrigin(origin);
    spImageData->GetDimensions(dim);
 
	int iBounds[6];
	WorldToVolumeBound(spImageData, bound, iBounds);

    auto spSubData = m_pDataModule->GetSubVolume(iBounds, spImageData);
    m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
    vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
    int z = iBounds[5] - iBounds[4] + 1;
    int h = iBounds[3] - iBounds[2] + 1;
    int w = iBounds[1] - iBounds[0] + 1;
    if (w > dim[0])
    {
        w = dim[0];
    }
    if (h > dim[1])
    {
        h = dim[1];
    }
    if (z > dim[2])
    {
        z = dim[2];
    }
    m_spDisplayImageData->SetDimensions(w, h, z);
    uchar ucChannel = 1;
    m_spDisplayImageData->SetScalarType(VTK_SHORT, info);

    double dSpacing[3];
    spImageData->GetSpacing(dSpacing);
    m_spDisplayImageData->SetSpacing(dSpacing);

    m_spDisplayImageData->SetNumberOfScalarComponents(ucChannel, info);//每个像素需要表示的组份 =1是指标量图
    m_spDisplayImageData->AllocateScalars(info);//很重要

    auto pDesData = m_spDisplayImageData->GetScalarPointer();
    auto pSorData = spSubData.get();
    auto totalSize = w * h * z * sizeof(short)*ucChannel;

    memcpy(pDesData, pSorData, totalSize);

    m_spDisplayImageData->UpdatePointGhostArrayCache();
    m_spDisplayImageData->UpdateCellGhostArrayCache();

    m_spVolumeMapper->SetInputData(m_spDisplayImageData.GetPointer());
}

ViewDirection Cell3D::GetViewDirection()
{
    return m_currentViewDir;
}

/*-----------------------------------------------------------
***   Description : 截屏
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::ScreenCut()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	QString filePathName = ScreenShotPath;
	filePathName.append("/");
	filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-zzz");
	filePathName += ".png";
	if (!screen->grabWindow(0).save(filePathName, "png"))
	{
		cout << "save full screen failed" << endl;
	}
}

/*-----------------------------------------------------------
***   Description : 截屏按钮响应函数
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::ScreenCapBtn()
{
	if (!m_bIsCap)
	{
		m_ScreenCap.StartCap();
		m_bIsCap = true;
	}
	else
	{
		m_ScreenCap.EndCap();
		m_bIsCap = false;
	}
}

void Cell3D::OnMeasure(int startPos[2], int endPos[2], double dLength)
{
	bool bValid = m_spTrackballCameraStyle->m_bMeasureEndInVolume && m_spTrackballCameraStyle->m_bMeasureStartInVolume;
	auto pSize = m_pRender->GetRenderWindow()->GetSize();
	double x1 = (double)startPos[0] / pSize[0];
	double y1 = (double)startPos[1] / pSize[1];
	double x2 = (double)endPos[0] / pSize[0];
	double y2 = (double)endPos[1] / pSize[1];

	m_spMeasureAxis->SetPoint1(x1, y1);
	m_spMeasureAxis->SetPoint2(x2,y2);

	//auto x = endPos[0] - startPos[0];
	//auto y = endPos[1] - startPos[1];
	//auto displayLength = sqrt(x*x + y * y);
	//auto worldLength = displayLength * GetPixelSize();
	if (bValid)
	{
		m_spMeasureAxis->m_bValidValue = true;
		m_spMeasureAxis->SetRulerDistance(1);
		m_spMeasureAxis->SetRange(0, dLength);
	}
	else
	{
		m_spMeasureAxis->SetRange(0, dLength);
		m_spMeasureAxis->m_bValidValue = false;
	}
	m_pRender->GetRenderWindow()->Render();
	m_spAxis->RenderOverlay(m_pRender);
	m_pRender->GetRenderWindow()->Render();
}

void Cell3D::EndMeasue(bool bHideRuler)
{
	m_currentAction = OperationAction::ActionPointer;
	auto pToolBar = m_spContainer->GetWidget<IPSToolsBar>(ksToolBarWidget);
	pToolBar->SetMeasureState(false);
	if (bHideRuler)
	{
		m_spMeasureAxis->VisibilityOff();
	}
}

void Cell3D::HideMeausureRuler()
{
	m_spMeasureAxis->VisibilityOff();
}

void Cell3D::UpdateZoom(float fZoom)
{
	__super::UpdateZoom(fZoom);
	if (m_spMeasureAxis->GetVisibility())
	{
		OnMeasure(m_spTrackballCameraStyle->m_iRulerStart, 
			m_spTrackballCameraStyle->m_iRulerEnd,
			m_spTrackballCameraStyle->GetRulerLength());
	}
}
 

void Cell3D::SetMeasure3D(bool bChecked)
{
	if (bChecked)
	{
		m_currentAction = OperationAction::ActionRuler;
		m_spMeasureAxis->SetPoint1(0, 0);
		m_spMeasureAxis->SetPoint2(0, 0);
		m_spMeasureAxis->VisibilityOn();
	}
	else
	{
		m_currentAction = OperationAction::ActionPointer;
		m_spMeasureAxis->VisibilityOff();
	}
}

void Cell3D::SetRegionColor(double bound[6])
{ 
	int dim[3];
	double spacing[3], origin[3];
	m_spDisplayImageData->GetSpacing(spacing);
	m_spDisplayImageData->GetOrigin(origin);
	m_spDisplayImageData->GetDimensions(dim);
	int iBounds[6];
	WorldToVolumeBound(m_spDisplayImageData, bound, iBounds);
	short* data = (short*)(m_spDisplayImageData->GetScalarPointer());

	//for (size_t i = 0; i < dim[0]* dim[1]* dim[2]; i++)
	//{
	//	data[i] = 0;
	//}

	m_vecUpdateList.clear();
	int z = iBounds[5] - iBounds[4] ;
	int h = iBounds[3] - iBounds[2] ;
	int w = iBounds[1] - iBounds[0] ;
    if (w > dim[0])
    {
        w = dim[0];
    }
    if (h > dim[1])
    {
        h = dim[1];
    }
    if (z > dim[2])
    {
        z = dim[2];
    }
	int64_t iSize = w * h*z;
	m_vecUpdateList.reserve(iSize);

	for (int k = iBounds[4]; k<= iBounds[5];++k)
	{
		for (int j = iBounds[2]; j <= iBounds[3]; ++j)
		{
			for (int i = iBounds[0]; i <= iBounds[1]; ++i)
			{
				int index = k * dim[0] * dim[1] + j * dim[0] + i;
				short& value = data[index];
				if (value <= 1000 && value >=0)
				{
					m_vecUpdateList.emplace_back(index, value, 0);
					value = 1;
				}
				else
				{
					m_vecUpdateList.emplace_back(index, value, 5000);
					value = 5000;
				}
			}
		}
	}



    vtkNew<vtkColorTransferFunction> colors;
    colors->AddRGBPoint(0.0, 0.0, 0.0, 0.1);
    colors->AddRGBPoint(1000.0, 0.0, 1.0, 0.0);
    colors->AddRGBPoint(2000.0, 1.0, 0.0, 0.0);
    colors->AddRGBPoint(3600.0, 0.0, 0.0, 1.0);
    colors->AddRGBPoint(4999.0, 1.0, 0.0, 0.0);
    colors->AddRGBPoint(5001.0, 1.0, 0.0, 0.0);

    float fAlpha = 0.3;
    vtkNew<vtkPiecewiseFunction> opacities;
    opacities->AddPoint(0.0, 0 * fAlpha);
    opacities->AddPoint(1000.0, 0.1 * fAlpha);
    opacities->AddPoint(2000.0, 0.80 * fAlpha);
    opacities->AddPoint(3600.0, -1.0 * fAlpha);
    opacities->AddPoint(4999.0, 0.0 * fAlpha);
    opacities->AddPoint(5001.0, 0.8);
                                                                   

    m_spVolumeProperty->SetScalarOpacity(opacities);

    m_spDisplayImageData->UpdatePointGhostArrayCache();
	m_spDisplayImageData->UpdateCellGhostArrayCache();

    m_spVolumeMapper->RemoveAllInputs();
	m_spVolumeMapper->SetInputData(m_spDisplayImageData);
	m_spVolumeMapper->Update();

    m_spDisplayImageData->ComputeBounds();
	auto spData = m_spVolumeMapper->GetInput();
	auto pData = spData->GetScalarPointer();
	m_pRender->RemoveVolume(m_pVolume);
	m_pRender->AddVolume(m_pVolume);
	m_pVolume->Update();
	m_pRender->GetRenderWindow()->Render();
}

void Cell3D::SelectRegion(double bounds[6])
{
   // CreatCroppingData(bounds, m_spDisplayImageData);

   // m_spDisplayImageData->GetBounds(bounds);
    // m_spBoxWidget->SetInputConnection(mapper->GetOutputPort());
	auto pToolBar = m_spContainer->GetWidget<IPSToolsBar>(ksToolBarWidget);
	auto spBoxItem = m_spTrackballCameraStyle->m_spBoxItem;
	if (fabs(bounds[0] - bounds[1]) < 1e-4)
	{//没有交集
		m_currentAction = OperationAction::ActionPointer;
		m_currentViewDir = ViewDirection::FrontView;
		pToolBar->SetClippingState(false);
		ResumeView();
		spBoxItem->m_spBoxWidget->Off();
		spBoxItem->m_isUsed = false;
		spBoxItem->m_isSelect = false;
		RecorverToOriginalValue();
		QMessageBox::information(NULL, "Waring", QString::fromLocal8Bit("选择的区域没有交集！"),
			QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	spBoxItem->m_spBoxRep->PlaceWidget(bounds);
    memcpy(spBoxItem->m_spBoxCallback->m_currentBounds, bounds, sizeof(double) * 6);
	spBoxItem->m_spBoxWidget->On();
    //m_spVolumeMapper->SetCroppingRegionPlanes(bounds);  

    if (m_spTrackballCameraStyle->m_iCroppingCount == 0)
    {
        //m_currentAction = OperationAction::ActionPointer;
        memcpy(m_spTrackballCameraStyle->m_defualtBounds, bounds, sizeof(double) * 6);
        m_currentViewDir = ViewDirection::TopView;
		SetRegionColor(bounds);
		TopView();
        m_spTrackballCameraStyle->m_iCroppingCount = 1;
       // m_currentAction = OperationAction::ActionPointer;
    }
    else
    {
        m_currentAction = OperationAction::ActionPointer;
        m_currentViewDir = ViewDirection::FrontView;
		pToolBar->SetClippingState(false);
		RecorverToOriginalValue();
		SetRegionColor(bounds);
		spBoxItem->m_isSelect = true;

        m_spAxesActor->SetVisibility(false);

		ResumeView();
		emit AddSuspicious(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5], spBoxItem->m_uid);
        m_spTrackballCameraStyle->m_iCroppingCount = 0;


    }
    //ExecuteClip();
}

void Cell3D::PageTra(int iIndex)
{
	if (!HasCTData())
	{
		return;
	}
    if (m_iPlaneDirection != 0)
    {
        m_planeWidget->SetPlaneOrientationToZAxes();
        m_planeWidget->PlaceWidget(m_pVolume->GetBounds());
        m_iPlaneDirection = 0;
        m_planeWidget->UpdatePlacement();
    }
  /*  double xyz[3];
    m_planeWidget->GetPoint1(xyz);
    double xyz2[3];
    m_planeWidget->GetPoint2(xyz2);
    double ori[3];
    m_planeWidget->GetOrigin(ori);

    double center[3];
    m_planeWidget->GetCenter(center);
*/
    double bounds[6];
    m_pVolume->GetBounds(bounds);
    double dPos = (double)iIndex / m_imageDims[2];
    dPos = dPos * bounds[5];
    m_planeWidget->SetSlicePosition(dPos);
    //m_sp3DRender->Render();
    auto pRenWin = this->GetRenderWindow();
    pRenWin->Render();
}

void Cell3D::PageSag(int iIndex)
{
	if (!HasCTData())
	{
		return;
	}
    //if (m_iPlaneDirection != 1)
    { //必须每次重新设置，否则会导致位置错误
        m_planeWidget->SetPlaneOrientationToYAxes();
        m_planeWidget->PlaceWidget(m_pVolume->GetBounds());
        m_planeWidget->UpdatePlacement();
        m_iPlaneDirection = 1;
    }
    double bounds[6];
    m_pVolume->GetBounds(bounds);
    double dPos = (double)iIndex / m_imageDims[1];
    dPos = dPos * bounds[1];
    m_planeWidget->SetSlicePosition(dPos);
    //m_sp3DRender->Render();
    auto pRenWin = this->GetRenderWindow();
    pRenWin->Render();
}

/*-----------------------------------------------------------
***   Description: 俯视图
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Cell3D::TopView()
{
    SetToFrontPos();
    auto spCamera = m_pRender->GetActiveCamera();
    spCamera->Elevation(90);

    //强制刷新 要不viewup不起作用，这里vtk有bug，通过下面的方式绕过去
    spCamera->SetViewUp(0.001, 0.0001, 1);
    spCamera->SetViewUp(0, 0, 1);
    spCamera->Modified();
    m_pRender->ResetCameraClippingRange();
    this->GetRenderWindow()->Render();
    m_currentViewDir = ViewDirection::TopView;
    HideMeausureRuler();
    m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
    UpdateZoom(1.0);
    return;
   /* SetToFrontPos();

    auto spCamera = m_pRender->GetActiveCamera();
    spCamera->Elevation(90);
    m_pRender->ResetCameraClippingRange();
    this->GetRenderWindow()->Render();
    m_currentViewDir = ViewDirection::TopView;
    HideMeausureRuler();
    m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
    UpdateZoom(1.0);
*/
	if (m_spDisplayImageData == nullptr)
	{
		return;
	}
    vtkCamera *pCamera = m_pRender->GetActiveCamera();

    double focal[3], currPos[3];// , bound[6];
    int cameraDist = pCamera->GetDistance();
    double viewAngle = pCamera->GetViewAngle();
    double angleTan = std::tan(vtkMath::RadiansFromDegrees(viewAngle / 2));
    double length = 0;

    int *winSize = this->GetRenderWindow()->GetSize();
    double radio = 1.0;
    if (winSize[1]> winSize[0])
    {
        radio = double(winSize[1]) / double(winSize[0]);
    }
    else
    {
        radio = double(winSize[0]) / double(winSize[1]);
    }
	radio = double(winSize[0]) / double(winSize[1]);

    double spacing[3], origin[3];
    int size[3];
	 
    auto imageData = m_spDisplayImageData;
    imageData->GetSpacing(spacing);
    imageData->GetOrigin(origin);
    imageData->GetDimensions(size);
    double* bound = imageData->GetBounds();

    focal[0] = (bound[0] + bound[1]) / 2;
    focal[1] = (bound[2] + bound[3]) / 2;
    focal[2] = (bound[4] + bound[5]) / 2;
	//focal[1] = 0;
    currPos[0] = focal[0];
    currPos[1] = focal[1];
    currPos[2] = focal[2];

    //length = std::max((bound[1] - bound[0])*radio, bound[5] - bound[4]);
	length = (bound[5] - bound[4])*radio;
	auto w = (bound[1] - bound[0]);
	int h = bound[5] - bound[4];
	if (w > h)
	{
		length = w;
	}
	else
	{
		length = h * radio;
	}
	//currPos[1] = -(focal[1] + length / 2 / angleTan)*g_fExtent;
	currPos[1] = (length / 2 / angleTan )*g_fExtent + focal[1] * 2;
	pCamera->SetViewUp(0, 0, 1);

    pCamera->SetPosition(currPos);
    pCamera->SetFocalPoint(focal);

    m_pRender->ResetCameraClippingRange();
	HideMeausureRuler();
	m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
	UpdateZoom(1.0);
}

void Cell3D::BottomView()
{
    SetToFrontPos();

    auto spCamera = m_pRender->GetActiveCamera();
    spCamera->Elevation(-90);

    //强制刷新 要不viewup不起作用，这里vtk有bug，通过下面的方式绕过去
    spCamera->SetViewUp(0.001, 0.0001, 1);
    spCamera->SetViewUp(0, 0, 1);
    spCamera->Modified();
    m_pRender->ResetCameraClippingRange();


    m_pRender->ResetCameraClippingRange();
    this->GetRenderWindow()->Render();
    m_currentViewDir = ViewDirection::BottomView;
    HideMeausureRuler();
    m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
    UpdateZoom(1.0);
}

/*-----------------------------------------------------------
***   Description : 左视图
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::LeftView()
{
	if (m_spDisplayImageData == nullptr)
	{
		return;
	}
	vtkCamera *pCamera = m_pRender->GetActiveCamera();

	double focal[3], currPos[3];// , bound[6];
	int cameraDist = pCamera->GetDistance();
	double viewAngle = pCamera->GetViewAngle();
	double angleTan = std::tan(vtkMath::RadiansFromDegrees(viewAngle / 2));
	double length = 0;

	int *winSize = this->GetRenderWindow()->GetSize();
	double radio = 1.0;
	if (winSize[1] > winSize[0])
	{
		radio = double(winSize[1]) / double(winSize[0]);
	}
	else
	{
		radio = double(winSize[0]) / double(winSize[1]);
	}
	radio = double(winSize[0]) / double(winSize[1]);

	double spacing[3], origin[3];
	int size[3];

	auto imageData = m_spDisplayImageData;
	imageData->GetSpacing(spacing);
	imageData->GetOrigin(origin);
	imageData->GetDimensions(size);
	double* bound = imageData->GetBounds();

	/*bound[0] = std::min(bound[0], origin[0]);
	bound[2] = std::min(bound[2], origin[1]);
	bound[4] = std::min(bound[4], origin[2]);

	bound[1] = std::max(bound[1], origin[0] + size[0] * spacing[0]);
	bound[3] = std::max(bound[3], origin[1] + size[1] * spacing[1]);
	bound[5] = std::max(bound[5], origin[2] + size[2] * spacing[2]);*/

	focal[0] = (bound[0] + bound[1]) / 2;
	focal[1] = (bound[2] + bound[3]) / 2;
	focal[2] = (bound[4] + bound[5]) / 2;

	currPos[0] = focal[0];
	currPos[1] = focal[1];
	currPos[2] = focal[2];
	auto w = bound[3] - bound[2];
	int h = bound[5] - bound[4];
	if (w > h)
	{
		length = w;
	}
	else
	{
		length = h * radio;
	}

		
	int objectWidth = bound[1] - bound[0];//物体宽度
	auto distance = length / 2 / angleTan * g_fExtent  + objectWidth / 2 ;
	auto pos = -(distance - focal[0]);//相机x坐标

	currPos[0] = pos ;


	pCamera->SetViewUp(0, 1, 0);

	pCamera->SetPosition(currPos);
	pCamera->SetFocalPoint(focal);

	m_pRender->ResetCameraClippingRange();
	HideMeausureRuler();
	m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
	UpdateZoom(1.0);
    m_currentViewDir = ViewDirection::LeftView;

}

/*-----------------------------------------------------------
***   Description : 右视图
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::RightView()
{
	if (m_spDisplayImageData == nullptr)
	{
		return;
	}
	vtkCamera *pCamera = m_pRender->GetActiveCamera();

	double focal[3], currPos[3];// , bound[6];
	int cameraDist = pCamera->GetDistance();
	double viewAngle = pCamera->GetViewAngle();
	double angleTan = std::tan(vtkMath::RadiansFromDegrees(viewAngle / 2));
	double length = 0;

	int *winSize = this->GetRenderWindow()->GetSize();
	double radio = 1.0;
	if (winSize[1] > winSize[0])
	{
		radio = double(winSize[1]) / double(winSize[0]);
	}
	else
	{
		radio = double(winSize[0]) / double(winSize[1]);

	}
	radio = double(winSize[0]) / double(winSize[1]);

	double spacing[3], origin[3];
	int size[3];

	auto imageData = m_spDisplayImageData;
	imageData->GetSpacing(spacing);
	imageData->GetOrigin(origin);
	imageData->GetDimensions(size);
	double* bound = imageData->GetBounds();

	//bound[0] = std::min(bound[0], origin[0]);
	//bound[2] = std::min(bound[2], origin[1]);
	//bound[4] = std::min(bound[4], origin[2]);

	//bound[1] = std::max(bound[1], origin[0] + size[0] * spacing[0]);
	//bound[3] = std::max(bound[3], origin[1] + size[1] * spacing[1]);
	//bound[5] = std::max(bound[5], origin[2] + size[2] * spacing[2]);

	focal[0] = (bound[0] + bound[1]) / 2;
	focal[1] = (bound[2] + bound[3]) / 2;
	focal[2] = (bound[4] + bound[5]) / 2;

	currPos[0] = focal[0];
	currPos[1] = focal[1];
	currPos[2] = focal[2];
	auto w = bound[3] - bound[2];
	int h = bound[5] - bound[4];
	if (w > h)
	{
		length = w;
	}
	else
	{
		length = h * radio;
	}


	//length = std::max((bound[3] - bound[2])*radio, bound[5] - bound[4]);
	//length / 2 / angleTan相机位置到焦点的距离。、
	//length / 2 / angleTan相机位置到焦点的距离+ focal[0]等于相机的到焦点的坐标值。
	//length / 2 / angleTan相机位置到焦点的距离+ focal[0] +  物体的长度的一半（等于focal[0]) 等于相机看右表面时候相机的坐标，
	//此时右表面充满窗口，窗口的大小表示物体的y方向的大小单位mm
	currPos[0] = (length / 2 / angleTan)*g_fExtent + focal[0] * 2 ;
	pCamera->SetViewUp(0, 1, 0);

	pCamera->SetPosition(currPos);
	pCamera->SetFocalPoint(focal);
	auto d = pCamera->GetDistance();
	auto a = d * angleTan * 2;
	m_pRender->ResetCameraClippingRange();
	HideMeausureRuler();
	m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
	UpdateZoom(1.0);
    m_currentViewDir = ViewDirection::RightView;
}

/*-----------------------------------------------------------
***   Description : 三维视图显示伪彩
***   OUTPUT      : 彩色视图
***   INPUT       : 
------------------------------------------------------------*/
void Cell3D::Color3D()
{
	if (!HasCTData())
	{
		return;
	}                                                      

    ResetOpacity();                                  
	// Create color property                          ;
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 有机物剔除
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::RemoveOrg3D()
{
    vtkNew<vtkColorTransferFunction> colors;
    colors->AddRGBPoint(0.0, 0.0, 0.1, 0.1);
    colors->AddRGBPoint(3000.0, 0.13, 0.81, 0.93);
    colors->AddRGBPoint(5000.0, 0.1, 0.1, 0.93);

    vtkNew<vtkPiecewiseFunction> opacities;
    opacities->AddPoint(0.0, 0.5);
    opacities->AddPoint(3000.0, 0.80);
    opacities->AddPoint(5000.0, 0.1);

    // Create color property
    m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    m_spVolumeProperty->SetInterpolationTypeToLinear();
    m_spVolumeProperty->ShadeOn();
    m_spVolumeProperty->SetAmbient(0.4);
    m_spVolumeProperty->SetDiffuse(0.6);
    m_spVolumeProperty->SetSpecular(0.2);
    m_spVolumeProperty->SetColor(colors);
    m_spVolumeProperty->SetScalarOpacity(opacities);
    m_pVolume->SetProperty(m_spVolumeProperty);
    m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 无机物剔除
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::RemoveInorg3D()
{
    vtkNew<vtkColorTransferFunction> colors;
    colors->AddRGBPoint(0.0, 0.1, 0.1, 0.0);
    colors->AddRGBPoint(3000.0, 0.13, 0.81, 0.93);
    colors->AddRGBPoint(5000.0, 0.1, 0.1, 0.93);

    vtkNew<vtkPiecewiseFunction> opacities;
    opacities->AddPoint(0.0, 0.5);
    opacities->AddPoint(3000.0, 0.80);
    opacities->AddPoint(5000.0, 0.1);

    // Create color property
    m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    m_spVolumeProperty->SetInterpolationTypeToLinear();
    m_spVolumeProperty->ShadeOn();
    m_spVolumeProperty->SetAmbient(0.4);
    m_spVolumeProperty->SetDiffuse(0.6);
    m_spVolumeProperty->SetSpecular(0.2);
    m_spVolumeProperty->SetColor(colors);
    m_spVolumeProperty->SetScalarOpacity(opacities);
    m_pVolume->SetProperty(m_spVolumeProperty);
    m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 三维边缘增强
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::EdgeEnhance3D()
{
	vtkNew<vtkColorTransferFunction> colors;
	colors->AddRGBPoint(0.0, 0.0, 0.1, 0.0);
	colors->AddRGBPoint(3000.0, 0.13, 0.81, 0.93);
	colors->AddRGBPoint(5000.0, 0.1, 0.1, 0.93);

	vtkNew<vtkPiecewiseFunction> opacities;
	opacities->AddPoint(0.0, -1.0);
	opacities->AddPoint(3000.0, 0.80);
	opacities->AddPoint(5000.0, 0.1);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 增加多重吸收率
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::IncreaseMultiAbsorb()
{
	vtkNew<vtkColorTransferFunction> colors;
	//colors->AddRGBPoint(3000.0, 0.93, 0.81, 0.63);
	colors->AddRGBPoint(2000.0, 0.33, 0.31, 0.93);
	colors->AddRGBPoint(4000.0, 0.3, 0.31, 0.8);

	vtkNew<vtkPiecewiseFunction> opacities;
	//opacities->AddPoint(3000.0, 0.30);
	opacities->AddPoint(2000.0, 0.3);
	opacities->AddPoint(4000.0, 0.3);
	// opacities->AddPoint(2000.0, 0.80);
	//opacities->AddPoint(3600.0, -1.0);
	//opacities->AddPoint(4999.0, 0.0);
	//opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 降低多重吸收率
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::DecreaseMultiAbsorb()
{
	vtkNew<vtkColorTransferFunction> colors;
	//colors->AddRGBPoint(3000.0, 0.93, 0.81, 0.63);
	colors->AddRGBPoint(0.00, 0.21, 0.53, 0.23);
	colors->AddRGBPoint(2000.0, 0.1, 0.31, 0.63);

	vtkNew<vtkPiecewiseFunction> opacities;
	//opacities->AddPoint(3000.0, 0.30);
	opacities->AddPoint(0.0, 0.3);
	opacities->AddPoint(2000.0, 0.3);
	// opacities->AddPoint(2000.0, 0.80);
	//opacities->AddPoint(3600.0, -1.0);
	//opacities->AddPoint(4999.0, 0.0);
	//opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}
/*-----------------------------------------------------------
***   Description : 三维低穿透
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::LightThrough3D()
{
	vtkNew<vtkColorTransferFunction> colors;
	//colors->AddRGBPoint(3000.0, 0.93, 0.81, 0.63);
	colors->AddRGBPoint(4000.00, 0.93, 0.81, 0.63);
	colors->AddRGBPoint(5000.0, 0.93, 0.81, 0.63);

	vtkNew<vtkPiecewiseFunction> opacities;
	//opacities->AddPoint(3000.0, 0.30);
	opacities->AddPoint(4000.0, 0.3);
	opacities->AddPoint(5000.0, 0.3);
	// opacities->AddPoint(2000.0, 0.80);
	//opacities->AddPoint(3600.0, -1.0);
	//opacities->AddPoint(4999.0, 0.0);
	//opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 三维高穿透
***   OUTPUT      : 
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::HavyThrough3D()
{
	if (!HasCTData())
	{
		return;
	}
	vtkNew<vtkColorTransferFunction> colors;
	colors->AddRGBPoint(-1000.0, 1.0, 0.0, 0.0);
	colors->AddRGBPoint(0.0, 0.93, 0.81, 0.63);
	colors->AddRGBPoint(1000.0, 0.1, 0.1, 0.1);
	//colors->AddRGBPoint(2000.0, 1.0, 0.0, 0.0);
	//colors->AddRGBPoint(3600.0, 0.0, 0.0, 1.0);
	//colors->AddRGBPoint(4999.0, 1.0, 0.0, 0.0);
	//colors->AddRGBPoint(5001.0, 1.0, 0.0, 0.0);

	vtkNew<vtkPiecewiseFunction> opacities;
	opacities->AddPoint(-1000.0, 0.80);
	opacities->AddPoint(0.0, 0.3);
	opacities->AddPoint(1000.0, 0.3);
	// opacities->AddPoint(2000.0, 0.80);
	//opacities->AddPoint(3600.0, -1.0);
	//opacities->AddPoint(4999.0, 0.0);
	//opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 三维视图反色
***   OUTPUT      : 黑白视图
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::NegativeProcess3D()
{
	if (!HasCTData())
	{
		return;
	}
	vtkNew<vtkColorTransferFunction> colors;
	colors->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
	colors->AddRGBPoint(1000.0, 0.0, 1.0, 0.0);
	colors->AddRGBPoint(2000.0, 1.0, 0.0, 0.0);
	colors->AddRGBPoint(3600.0, 0.0, 0.0, 1.0);
	colors->AddRGBPoint(4999.0, 1.0, 0.0, 0.0);
	colors->AddRGBPoint(5001.0, 1.0, 0.0, 0.0);

	vtkNew<vtkPiecewiseFunction> opacities;
	opacities->AddPoint(0.0, 0.9);
	opacities->AddPoint(1000.0, 0.9);
	opacities->AddPoint(2000.0, 0.20);
	opacities->AddPoint(3600.0, -1.0);
	opacities->AddPoint(4999.0, 0.0);
	opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
	m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}

/*-----------------------------------------------------------
***   Description : 三维视图显示黑白
***   OUTPUT      : 黑白视图
***   INPUT       : 
------------------------------------------------------------*/
void Cell3D::Gray3D()
{
	if (!HasCTData())
	{
		return;
	}
	vtkNew<vtkColorTransferFunction> colors;
	colors->AddRGBPoint(0.0, 1.0, 1.0, 1.0);
	colors->AddRGBPoint(1000.0, 1.0, 1.0, 1.0);
	colors->AddRGBPoint(2000.0, 1.0, 1.0, 1.0);
	colors->AddRGBPoint(3600.0, 1.0, 1.0, 1.0);
	colors->AddRGBPoint(4999.0, 1.0, 1.0, 1.0);
	colors->AddRGBPoint(5001.0, 1.0, 1.0, 1.0);

	vtkNew<vtkPiecewiseFunction> opacities;
	opacities->AddPoint(0.0, 0.1);
	opacities->AddPoint(1000.0, 0.1);
	opacities->AddPoint(2000.0, 0.80);
	opacities->AddPoint(3600.0, -1.0);
	opacities->AddPoint(4999.0, 0.0);
	opacities->AddPoint(5001.0, 0.0);

	// Create color property
	m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	m_spVolumeProperty->SetInterpolationTypeToLinear();
	m_spVolumeProperty->ShadeOn();
	m_spVolumeProperty->SetAmbient(0.4);
	m_spVolumeProperty->SetDiffuse(0.6);
	m_spVolumeProperty->SetSpecular(0.2);
	m_spVolumeProperty->SetColor(colors);
	m_spVolumeProperty->SetScalarOpacity(opacities);
    m_pVolume->SetProperty(m_spVolumeProperty);
	m_pRender->GetRenderWindow()->Render();
}


void Cell3D::SetToFrontPos()
{
    vtkCamera *pCamera = m_pRender->GetActiveCamera();
    if (m_spDisplayImageData == nullptr)
    {
        return;
    }
    m_planeWidget->SetPlaneOrientationToZAxes();
    m_planeWidget->PlaceWidget(m_pVolume->GetBounds());

    double focal[3], currPos[3];// , bound[6];
    int cameraDist = pCamera->GetDistance();
    double viewAngle = pCamera->GetViewAngle();
    double angleTan = std::tan(vtkMath::RadiansFromDegrees(viewAngle / 2));
    double length = 0;

    int *winSize = this->GetRenderWindow()->GetSize();
    double radio = 1.0;
    if (winSize[1] > winSize[0])
    {
        radio = double(winSize[1]) / double(winSize[0]);
    }
    else
    {
        radio = double(winSize[0]) / double(winSize[1]);

    }
    radio = double(winSize[0]) / double(winSize[1]);

    double spacing[3], origin[3];
    int size[3];

    auto imageData = m_spDisplayImageData;
    imageData->GetSpacing(spacing);
    imageData->GetOrigin(origin);
    imageData->GetDimensions(size);
    double* bound = imageData->GetBounds();

    focal[0] = (bound[0] + bound[1]) / 2;
    focal[1] = (bound[2] + bound[3]) / 2;
    focal[2] = (bound[4] + bound[5]) / 2;

    currPos[0] = focal[0];
    currPos[1] = focal[1];
    currPos[2] = focal[2];
    currPos[1] = focal[1];

    auto w = (bound[1] - bound[0]);
    int h = bound[3] - bound[2];
    if (fabs(w - h) < 1e-4)
    {
        length = w;
    }
    else
    {
        length = h * radio;
    }
    currPos[2] = (length / 2 / angleTan + focal[2] * 2)*g_fExtent;
    m_iLength = length;
    pCamera->SetViewUp(0, 1, 0);
    pCamera->SetPosition(currPos);
    pCamera->SetFocalPoint(focal);
}

/*-----------------------------------------------------------
***   Description : 恢复视图
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void Cell3D::ResumeView()
{
    SetToFrontPos();
    m_pRender->ResetCameraClippingRange();
    this->GetRenderWindow()->Render();
    m_currentViewDir = ViewDirection::FrontView;
	HideMeausureRuler();
	m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
	UpdateZoom(1.0);
}
void Cell3D::RearView()
{
    SetToFrontPos();
    auto spCamera = m_pRender->GetActiveCamera();
    spCamera->Elevation(-180);
    //强制刷新 要不viewup不起作用，这里vtk有bug，通过下面的方式绕过去
    spCamera->SetViewUp(0.001, 1.01, 0.0001);
    spCamera->SetViewUp(0, 1, 0);                    
    spCamera->Modified();
    m_pRender->ResetCameraClippingRange();
    this->GetRenderWindow()->Render();
    m_currentViewDir = ViewDirection::RearView;
    HideMeausureRuler();
    m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
    UpdateZoom(1.0);
}

/*-----------------------------------------------------------
***   Description:    窗口变化函数
***   OUTPUT     :
***   INPUT      : event：消息内容
------------------------------------------------------------*/
void Cell3D::resizeEvent(QResizeEvent * event)
{
    auto pActieSie = m_pRender->GetRenderWindow()->GetActualSize();
    auto rect = this->geometry();
   
    m_pRender->ResetCameraClippingRange();
   // m_pRender->ResetCamera();
    //需要重新设置大小
    m_spInteractor->UpdateSize(rect.width(), rect.height());
    m_pRender->GetRenderWindow()->SetSize(rect.width(), rect.height());
    m_pRender->GetRenderWindow()->StereoCapableWindowOn();
	GetRenderWindow()->SetSize(rect.width(), rect.height());
    if (m_spDisplayImageData == nullptr)
    {
        return;
    }




    //double center[3];
    //double distance;
    //double vn[3], *vup;

    //auto pCameraa = m_pRender->GetActiveCamera();
    //if (m_pRender->GetActiveCamera() != nullptr)
    //{
    //    pCameraa->GetViewPlaneNormal(vn);
    //}
    //else
    //{
    //    return;
    //}

    //// Reset the perspective zoom factors, otherwise subsequent zooms will cause
    //// the view angle to become very small and cause bad depth sorting.
    //pCameraa->SetViewAngle(30.0);
    //double bounds[6];
    //m_spDisplayImageData->GetBounds(bounds);

    ////m_pRender->ExpandBounds(bounds, pCameraa->GetModelTransformMatrix());

    //center[0] = (bounds[0] + bounds[1]) / 2.0;
    //center[1] = (bounds[2] + bounds[3]) / 2.0;
    //center[2] = (bounds[4] + bounds[5]) / 2.0;

    //double w1 = bounds[1] - bounds[0];
    //double w2 = bounds[3] - bounds[2];
    //double w3 = bounds[5] - bounds[4];
    //w1 *= w1;
    //w2 *= w2;
    //w3 *= w3;
    //double radius = w1 + w2 + w3;

    //// If we have just a single point, pick a radius of 1.0
    //radius = (radius == 0) ? (1.0) : (radius);

    //// compute the radius of the enclosing sphere
    //radius = sqrt(radius)*0.5;

    //// default so that the bounding sphere fits within the view fustrum

    //// compute the distance from the intersection of the view frustum with the
    //// bounding sphere. Basically in 2D draw a circle representing the bounding
    //// sphere in 2D then draw a horizontal line going out from the center of
    //// the circle. That is the camera view. Then draw a line from the camera
    //// position to the point where it intersects the circle. (it will be tangent
    //// to the circle at this point, this is important, only go to the tangent
    //// point, do not draw all the way to the view plane). Then draw the radius
    //// from the tangent point to the center of the circle. You will note that
    //// this forms a right triangle with one side being the radius, another being
    //// the target distance for the camera, then just find the target dist using
    //// a sin.
    //double angle = vtkMath::RadiansFromDegrees(pCameraa->GetViewAngle());
    //double parallelScale = radius;

    //m_pRender->ComputeAspect();
    //double aspect[2];
    //m_pRender->GetAspect(aspect);

    //if (aspect[0] >= 1.0) // horizontal window, deal with vertical angle|scale
    //{
    //    if (pCameraa->GetUseHorizontalViewAngle())
    //    {
    //        angle = 2.0*atan(tan(angle*0.5) / aspect[0]);
    //    }
    //}
    //else // vertical window, deal with horizontal angle|scale
    //{
    //    if (!pCameraa->GetUseHorizontalViewAngle())
    //    {
    //        angle = 2.0*atan(tan(angle*0.5)*aspect[0]);
    //    }

    //    parallelScale = parallelScale / aspect[0];
    //}

    //distance = radius / sin(angle*0.5);

    //// check view-up vector against view plane normal
    //vup = pCameraa->GetViewUp();
    //if (fabs(vtkMath::Dot(vup, vn)) > 0.999)
    //{
    //    pCameraa->SetViewUp(-vup[2], vup[0], vup[1]);
    //}

    //// update the camera
    //pCameraa->SetFocalPoint(center[0], center[1], center[2]);
    //pCameraa->SetPosition(center[0] + distance * vn[0],
    //    center[1] + distance * vn[1],
    //    center[2] + distance * vn[2]);

    //m_pRender->ResetCameraClippingRange(bounds);

    //// setup default parallel scale
    //pCameraa->SetParallelScale(parallelScale);

    //auto d2 = m_pRender->GetActiveCamera()->GetDistance();;
    m_spTrackballCameraStyle->m_dDefaultDistance = m_pRender->GetActiveCamera()->GetDistance();
                          
    int x = rect.width() - 10;
    int iHeight = 300;
    int y = (rect.height() - iHeight) / 2;
    m_spSliderRep2D->GetPoint1Coordinate()->SetCoordinateSystemToDisplay();
    m_spSliderRep2D->GetPoint1Coordinate()->SetValue(x, y);
    m_spSliderRep2D->GetPoint2Coordinate()->SetCoordinateSystemToDisplay();
    m_spSliderRep2D->GetPoint2Coordinate()->SetValue(x, y + iHeight);           

    ResumeView();

}

//**************************************************
//	brief   :重载窗口显示的消息，用来设置vtk的interaction的窗口大小
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell3D::showEvent(QShowEvent *event)
{
    __super::showEvent(event);
}

vtkSmartPointer<vtkImageData> Cell3D::GetImageData()
{
    auto spImageData = m_pDataModule->GetOriginalImageData();
    return  spImageData;
}
/*-----------------------------------------------------------
***   Description:点击裁剪按钮执行函数
***   OUTPUT     :
***   INPUT      :bCropping:是否是裁剪操作
------------------------------------------------------------*/
void Cell3D::OnCropping(bool bCropping)
{
	if (!this->isVisible())
	{
		return;
	}
	if (!HasCTData())
	{
		return;
	}
    if (bCropping)
    {
        ResetOpacity();
        m_currentAction = OperationAction::ActionCropping;
        auto pCroppingCell = m_spContainer->GetWidget<Cell3D>(ksCroppintCellWidget);
        pCroppingCell->hide();
        //auto bounds = m_spDisplayImageData->GetBounds();
		double bounds[6];
		GetMyBound(m_spDisplayImageData, bounds);
		memcpy(m_spTrackballCameraStyle->m_defualtBounds, bounds, sizeof(double) * 6);
        m_spTrackballCameraStyle->m_iCroppingCount = 0;
		auto spBoxWidgetItem = GetBoxWidgetItem();
		spBoxWidgetItem->m_isUsed = true;
		m_spTrackballCameraStyle->m_spBoxItem = spBoxWidgetItem;
		spBoxWidgetItem->m_spBoxRep->PlaceWidget(bounds);
        memcpy(spBoxWidgetItem->m_spBoxCallback->m_currentBounds, bounds, sizeof(double) * 6);

        //m_spVolumeMapper->SetCroppingRegionPlanes(bounds);
       // m_spVolumeMapper->SetInputData(m_spDisplayImageData.GetPointer());
		spBoxWidgetItem->m_spBoxWidget->Off();
		RecorverToOriginalValue();
		HideMeausureRuler();
		for each (auto item in m_vecBoxWidgetItem)
		{
			item->m_isSelect = false;
		}
        m_spAxesActor->SetVisibility(true);

        ResumeView();
    }
    else
    {
        m_currentAction = OperationAction::ActionPointer;
    }
}

void Cell3D::SelectSuspiciousObject(QString uid)
{
	RecorverToOriginalValue();

	double bound[6];
	for each (auto item in m_vecBoxWidgetItem)
	{
		if (item->m_uid == uid)
		{
			item->m_spBoxCallback->GetCurrentBounds(bound);
			item->m_isSelect = true;
		}
		else
		{
			item->m_isSelect = false;
		}
	}

	SetRegionColor(bound);
}

void Cell3D::RemoveSuspiciousObject(QString uid)
{
	for each (auto item in m_vecBoxWidgetItem)
	{
		if (item->m_uid == uid)
		{
			item->m_isUsed = false;
			item->m_spBoxWidget->Off();
			if (item->m_isSelect)
			{
				item->m_isSelect = false;
				RecorverToOriginalValue();
			}
			return;
		}
	}
	m_spRenderWindow->Render();
}

void Cell3D::ClearIamge()
{
	m_pDataModule->ClearData();
	m_planeWidget->Off();

	m_spDisplayImageData = nullptr;

    for each (auto item in m_vecBoxWidgetItem)
    {
        item->m_isSelect = false;
        item->m_isUsed = false;
        item->m_spBoxWidget->Off();                     
    }

	m_spVolumeMapper->SetInputData(m_spDisplayImageData);
	m_spRenderWindow->Render();
}


void Cell3D::WorldToVolumeBound(vtkSmartPointer<vtkImageData> spImageData, const double worldBound[6], int volumeBound[6])
{
	double xMin = worldBound[0];
	double xMax = worldBound[1];
	double yMin = worldBound[2];
	double yMax = worldBound[3];
	double zMin = worldBound[4];
	double zMax = worldBound[5];
	double spacing[3];
	spImageData->GetSpacing(spacing);
	int dim[3];
    double dBounds[6];
    m_pVolume->GetBounds(dBounds);
	spImageData->GetDimensions(dim);
	volumeBound[0] = std::max(0, (int)(xMin / spacing[0]));
	volumeBound[1] = std::min((int)(xMax / spacing[0]), dim[0]);
	

	volumeBound[2] = std::max(0, (int)(yMin / spacing[1]));
	volumeBound[3] = std::min((int)(yMax / spacing[1]), dim[1]);
	
	volumeBound[4] = std::max(0, (int)(zMin / spacing[2]));
	volumeBound[5] = std::min((int)(zMax / spacing[2]), dim[2]);




	volumeBound[0] = volumeBound[0] < 0 ? 0 : volumeBound[0];
	volumeBound[0] = volumeBound[0] > dim[0]-1 ? dim[0]-1 : volumeBound[0];

	volumeBound[1] = volumeBound[1] > dim[0]-1 ? dim[0]-1 : volumeBound[1];
	volumeBound[1] = volumeBound[1] < 0 ? 0 : volumeBound[1];

	volumeBound[2] = volumeBound[2] < 0 ? 0 : volumeBound[2];
	volumeBound[2] = volumeBound[2] > dim[1] - 1 ? dim[1] - 1 : volumeBound[2];

	volumeBound[3] = volumeBound[3] < 0 ? 0 : volumeBound[3];
	volumeBound[3] = volumeBound[3] > dim[1]-1 ? dim[1] - 1 : volumeBound[3];

	volumeBound[4] = volumeBound[4] < 0 ? 0 : volumeBound[4];
	volumeBound[4] = volumeBound[4] >= dim[2]-1 ? dim[2]-1 : volumeBound[4];

	volumeBound[5] = volumeBound[5] < 0 ? 0 : volumeBound[5];
	volumeBound[5] = volumeBound[5] >= dim[2] - 1 ? dim[2]-1 : volumeBound[5];

	if (volumeBound[0] > volumeBound[1])
	{
		std::swap(volumeBound[0], volumeBound[1]);
	}
	if (volumeBound[2] > volumeBound[3])
	{
		std::swap(volumeBound[2], volumeBound[3]);
	}
	if (volumeBound[4] > volumeBound[5])
	{
		std::swap(volumeBound[4], volumeBound[5]);
	}
}

void Cell3D::RecorverToOriginalValue()
{
	short* pData = (short*)(m_spDisplayImageData->GetScalarPointer());
	for each (auto item in m_vecUpdateList)
	{
		pData[item.index] = item.oriValue;
	}
	m_pRender->RemoveVolume(m_pVolume);
	m_pRender->AddVolume(m_pVolume);
}

std::shared_ptr<BoxWidgetItem> Cell3D::GetBoxWidgetItem()
{
	for each (auto item in m_vecBoxWidgetItem)
	{
		if (!item->m_isUsed)
		{
			return item;
		}
	}
	auto spNewItem = std::make_shared<BoxWidgetItem>();
	InitlizeBoxWidget(spNewItem);
	m_vecBoxWidgetItem.push_back(spNewItem);
	return spNewItem;
}

vector<std::shared_ptr<BoxWidgetItem>> Cell3D::GetAllBoxWidgetItem()
{
	return m_vecBoxWidgetItem;
}

void Cell3D::GetMyBound(vtkSmartPointer<vtkImageData> spImageData, double bound[6])
{
    m_pVolume->GetBounds(bound);
    double bounds[6];
    spImageData->GetBounds(bounds);
	/*double spacing[3], origin[3];
	int size[3];

	spImageData->GetSpacing(spacing);
	spImageData->GetOrigin(origin);
	spImageData->GetDimensions(size);

	bound[0] = origin[0];
	bound[2] = origin[1];
	bound[4] = origin[2];

	bound[1] = origin[0] + size[0] * spacing[0];
	bound[3] = origin[1] + size[1] * spacing[1];
	bound[5] = origin[2] + size[2] * spacing[2];*/
}

void Cell3D::ResetOpacity()
{
    vtkNew<vtkColorTransferFunction> colors;
    colors->AddRGBPoint(0.0, 0.0, 0.0, 0.1);
    colors->AddRGBPoint(1000.0, 0.0, 1.0, 0.0);
    colors->AddRGBPoint(2000.0, 1.0, 0.0, 0.0);
    colors->AddRGBPoint(3600.0, 0.0, 0.0, 1.0);
    colors->AddRGBPoint(4999.0, 1.0, 0.0, 0.0);
    colors->AddRGBPoint(5001.0, 1.0, 0.0, 0.0);

    // Create color property
    m_spVolumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    m_spVolumeProperty->SetInterpolationTypeToLinear();
    m_spVolumeProperty->ShadeOn();
    m_spVolumeProperty->SetAmbient(0.4);
    m_spVolumeProperty->SetDiffuse(0.6);
    m_spVolumeProperty->SetSpecular(0.2);
    m_spVolumeProperty->SetColor(colors);

    vtkNew<vtkPiecewiseFunction> opacities;
    //opacities->AddPoint(0.0, 0);
    //opacities->AddPoint(1000.0, 1);
    //opacities->AddPoint(2000.0, 1);
    //opacities->AddPoint(3600.0, 1);
    //opacities->AddPoint(4999.0, 1);
    //opacities->AddPoint(5001.0, 1);

    opacities->AddPoint(0.0, 0);
    opacities->AddPoint(1000.0, 0.1);
    opacities->AddPoint(2000.0, 0.80);
    opacities->AddPoint(3600.0, -1.0);
    opacities->AddPoint(4999.0, 0.0);
    opacities->AddPoint(5001.0, 0.0);

    m_spVolumeProperty->SetScalarOpacity(opacities);
    m_pVolume->SetProperty(m_spVolumeProperty);
    m_pRender->GetRenderWindow()->Render();

}

void Cell3D::ClickAxis(bool & bX, bool & bY, bool & bZ)
{
    bool bClickAxis = bX || bY || bZ;
    m_spTrackballCameraStyle->m_bClickAxis = bClickAxis;

    if (m_currentViewDir == ViewDirection::FrontView)
    {
        if (bX)
        {
            LeftView();
        }
        if (bY)
        {
            BottomView();
        }
        if (bZ)
        {
            RearView();
        }

    }
    else if (m_currentViewDir == ViewDirection::RearView)
    {
        if (bX)
        {
            RightView();
        }
        if (bY)
        {
            BottomView();
        }
        if (bZ)
        {
            ResumeView();
        }
    }

    else  if (m_currentViewDir == ViewDirection::BottomView)
    {
        if (bX)
        {
            LeftView();
        }
        if (bY)
        {
            TopView();
        }
        if (bZ)
        {
            ResumeView();
        }
    }
    else if (m_currentViewDir == ViewDirection::TopView)
    {
        if (bX)
        {
            RightView();
        }
        if (bY)
        {
            BottomView();
        }
        if (bZ)
        {
            ResumeView();
        }
    }
    
    else if (m_currentViewDir == ViewDirection::LeftView)
    {
        if (bX)
        {
            RightView();
        }
        if (bY)
        {
            BottomView();
        }
        if (bZ)
        {
            ResumeView();
        }
    }

    else if (m_currentViewDir == ViewDirection::RightView)
    {
        if (bX)
        {
            LeftView();
        }
        if (bY)
        {
            BottomView();
        }
        if (bZ)
        {
            ResumeView();
        }
    }
    
}

double Cell3D::GetPixelSize()
{
	vtkCamera *pCamera = m_pRender->GetActiveCamera();
	double viewAngle = pCamera->GetViewAngle();
	double angleTan = std::tan(vtkMath::RadiansFromDegrees(viewAngle / 2));
	int *winSize = this->GetRenderWindow()->GetSize();
    double displayLength = winSize[0];
	double radio = 1.0;
	auto d = pCamera->GetDistance();
 
	auto w = d * angleTan * 2;
	
	 m_pRender->ComputeAspect();
	 double aspect[2];
	 m_pRender->GetAspect(aspect);


	 if (aspect[0] >= 1.0) // horizontal window, deal with vertical angle|scale
	 {
	   w /= aspect[0];
	 }
	 else // vertical window, deal with horizontal angle|scale
	 {
	   w *= aspect[0];
	 }


     auto spPicker = (vtkCellPicker*)this->m_spInteractor->GetPicker();
     spPicker->Modified();               
     m_pRender->Modified();
     m_pRender->GetActiveCamera()->Modified();
     int x_1 = winSize[0] / 2 + 50;
     int y_1 = winSize[1] / 2 + 50;
     auto bMeasureEndInVolume = spPicker->Pick(x_1, y_1, 0, m_pRender);
     double m_dRulerStartWord[3];
     double m_dRulerEndWord[3];
     spPicker->GetPickPosition(m_dRulerEndWord);

     int x_2 = winSize[0] / 2 - 50;
     int y_2 = winSize[1] / 2 - 50;
     auto bMeasureStartInVolume = spPicker->Pick(x_2, y_2, 0, m_pRender);
     spPicker->GetPickPosition(m_dRulerStartWord);
     auto x = m_dRulerEndWord[0] - m_dRulerStartWord[0];
     auto y = m_dRulerEndWord[1] - m_dRulerStartWord[1];
     auto z = m_dRulerEndWord[2] - m_dRulerStartWord[2];
     if (bMeasureEndInVolume&&bMeasureStartInVolume)
     {
         w = sqrt(x*x + y * y + z * z);
         displayLength = sqrt(100 * 100 * 2);
     }
	auto fPxicelSize = (float)w / displayLength;
	return fPxicelSize;
}