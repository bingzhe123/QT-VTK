#include "Cell2D.h"
#include "commondef.h"
#include <vtkRenderer.h>
#include "vtkInformation.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToColors.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include <vtkPNGReader.h>//PNG读取类
#include "vtkCamera.h"
#include "vtkInteractorStyleImage.h"
#include "DataModule.h"
#include "vtkTexturedButtonRepresentation2D.h"
#include "vtkButtonWidget.h"
#include "vtkRenderWindow.h"
#include "vtkProp3DButtonRepresentation.h"
#include "vtkRendererCollection.h"
#include "CustomStyleImage.h"
#include "IPSImageAlgo.h"

#include <qdebug>

vtkStandardNewMacro(CustomStyleImage);

Cell2D::Cell2D(ContainerPtr spContainer, QWidget *pMainWnd):
    Cell(spContainer, pMainWnd)
{
    m_sCellName = "Cell2D";
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::Cell2D");
    m_spImageViewer = vtkSmartPointer<vtkImageViewer2>::New();

    //auto pRenderWindow = m_spImageViewer->GetRenderWindow();
    auto pRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_spImageViewer->SetRenderWindow(pRenderWindow);
    m_spRenderWindow = pRenderWindow;
    m_spImageViewer->GetRenderer()->SetBackground(255, 255, 255);
    this->SetRenderWindow(pRenderWindow);
    m_spInteractor->SetRenderWindow(pRenderWindow);

    pRenderWindow->SetInteractor(m_spInteractor);
    m_pRender = m_spImageViewer->GetRenderer();
	m_spImageViewer->SetRenderer(m_pRender);

    m_pRender->AddViewProp(m_spCornerAnnotationBottomRight);
    m_pRulerRender = m_pRender;

    m_spStyleImage = vtkSmartPointer<CustomStyleImage>::New();
    m_spInteractor->SetInteractorStyle(m_spStyleImage);
    m_spStyleImage->m_spImageViewer = m_spImageViewer;
    m_spStyleImage->m_pCurrentCell = this;
    m_spStyleImage->m_updateZoomFun = [this](float zoom)
    {
        UpdateZoom(zoom);
    };
    m_spStyleImage->m_HideRulerFun = [this](void)
    {
        this->HideRuler();
    };
   // std::string s("D:\\dcm");
    /*std::string folder = "..//..//..//code//test_data//test";
    m_spImportImagesData = std::make_shared< ImportImagesData>(folder.c_str();*/

   /* double dSpacex, dSpacey, dSpacez;
    m_spImportImagesData->GetImagesData(m_pData, m_w, m_h, m_z, dSpacex, dSpacey, dSpacez);
    m_spImageData = m_spImportImagesData->GetImageData();
    m_pData = (short *) m_spImageData->GetScalarPointer();*/

   // SetImage(m_pData, m_w, m_h);;

    //ui.pushButton->setStyleSheet("QPushButton{border-image:url(:/img/res/arrowup.png);}");

	vtkCamera* pCamera = m_pRender->GetActiveCamera();
	pCamera->ParallelProjectionOn();
    m_pRender->AddActor2D(m_spAxis);
	m_spAxis->VisibilityOff();
	m_spCornerAnnotationBottomRight->VisibilityOff();
	string sPath = "res/white.png";
	LoadPng(sPath);

	m_iIndex = -1;
	m_eOrg = UNKNOWN;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::Cell2D");
}

 
Cell2D::~Cell2D()
{

}

void Cell2D::SetImageViewer(vtkSmartPointer<vtkImageViewer2> spImageView)
{
    if (spImageView == nullptr)
    {
        m_spImageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    }
    else
    {
        m_spImageViewer = spImageView;
    }
    auto pRenderWindow = m_spImageViewer->GetRenderWindow();

    m_spRenderWindow = pRenderWindow;
    m_spImageViewer->GetRenderer()->SetBackground(255, 255, 255);
    this->SetRenderWindow(pRenderWindow);
    m_spInteractor->SetRenderWindow(pRenderWindow);

    pRenderWindow->SetInteractor(m_spInteractor);
    m_pRender = m_spImageViewer->GetRenderer();
    m_pRender->AddViewProp(m_spCornerAnnotationBottomRight);
    m_pRulerRender = m_pRender;

    m_spStyleImage = vtkSmartPointer<CustomStyleImage>::New();
    m_spInteractor->SetInteractorStyle(m_spStyleImage);
    m_spStyleImage->m_spImageViewer = m_spImageViewer;
    m_spStyleImage->m_pCurrentCell = this;
    m_spStyleImage->m_updateZoomFun = [this](float zoom)
    {
        UpdateZoom(zoom);
    };
    m_spStyleImage->m_HideRulerFun = [this](void)
    {
        this->HideRuler();
    };
    m_pRender->AddActor2D(m_spAxis);
}


void Cell2D::Initialize()
{
  /*  auto pRenderWindow = m_spImageViewer->GetRenderWindow();
    m_spImageViewer->GetRenderer()->SetBackground(0, 0, 128);
    this->SetRenderWindow(pRenderWindow);
    auto pInterStyle = m_spImageViewer->GetInteractorStyle();
    m_spStyleImage = vtkSmartPointer<CustomStyleImage>::New();
    m_spInteractor->SetRenderWindow(pRenderWindow);
    m_spInteractor->SetInteractorStyle(m_spStyleImage);
    pRenderWindow->SetInteractor(m_spInteractor);
    auto *size = pRenderWindow->GetSize();
    m_spInteractor->UpdateSize(size[0], size[1]);

    m_spStyleImage->m_spImageViewer = m_spImageViewer;
*/
}

void Cell2D::RefreshImage(unsigned char * pData, int w, int h)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::RefreshImage");
    m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
    vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
    m_spDisplayImageData->SetDimensions(w, h, 1);
    uchar ucChannel = 4;
    m_spDisplayImageData->SetScalarType(VTK_UNSIGNED_CHAR, info);

	auto spDataModule = m_pDataModule;
    double dSpacing[3];
    auto spImageData = spDataModule->GetOriginalImageData();
    spImageData->GetSpacing(dSpacing);
	if (m_eOrg == SAG)
	{
		dSpacing[1] = dSpacing[2];
		dSpacing[2] = 0.0;
		m_spDisplayImageData->SetSpacing(dSpacing);

	}
	else
	{
		m_spDisplayImageData->SetSpacing(dSpacing);

	}

    m_spDisplayImageData->SetNumberOfScalarComponents(ucChannel, info);//每个像素需要表示的组份 =1是指标量图
    m_spDisplayImageData->AllocateScalars(info);//很重要

    auto pDesData = m_spDisplayImageData->GetScalarPointer();
    auto pSorData = pData;
    auto totalSize = w * h * sizeof(uchar)*ucChannel;
    memcpy(pDesData, pSorData, totalSize);
    m_spDisplayImageData->UpdatePointGhostArrayCache();
    m_spDisplayImageData->UpdateCellGhostArrayCache();
    m_spImageViewer->SetInputData(m_spDisplayImageData.GetPointer());
    m_spImageViewer->UpdateDisplayExtent();
 
    auto spRender = m_spImageViewer->GetRenderer();
    vtkCamera* pCamera = spRender->GetActiveCamera();
    pCamera->ParallelProjectionOn();
	m_spImageViewer->Render();

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::RefreshImage");
}
vtkSmartPointer<vtkImageData> Cell2D::GetImageData()
{
    return m_spDisplayImageData;
}

double Cell2D::GetPixelSize()
{
    int *dim = GetImageData()->GetDimensions();
    vtkSmartPointer<vtkCoordinate> c = vtkCoordinate::New();
    double *pSpace = GetImageData()->GetSpacing();
    c->SetCoordinateSystemToWorld();
    auto w = dim[0] * pSpace[0];
    c->SetValue(w, 0, 0);
    auto res = c->GetComputedDisplayValue(m_pRender);
    auto endPos = res[0];
    c->SetValue(0, 0, 0);
    res = c->GetComputedDisplayValue(m_pRender);
    auto oriPos = res[0];
    return w/(endPos - oriPos);
}

void Cell2D::ClearIamge()
{
	m_pDataModule->ClearData();
	m_spDisplayDataPixel = nullptr;
	m_spDisplayImageData = nullptr;
	string sPath = "res/white.png";
	LoadPng(sPath);
}

//**************************************************
//	brief   : CT图像低穿透处理
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::LightThroughCT()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::LightThroughCT");
	if (!HasCTData())
	{
		return;
	}

	int size = m_w * m_h;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	heavyThrough(m_spOriginalDataPixel.get(), 3000, 3000, m_w, m_h, tempData.get());
	CTColor((const unsigned short*)tempData.get(), m_w, m_h, m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
	FitWindow();
}

//**************************************************
//	brief   : CT图像高穿透处理
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::HavyThroughCT()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::HavyThroughCT");
	//auto spDataModule = m_spContainer->GetModule<DataModule>(ksDataModule);
	//m_spDisplayImageData = spDataModule->GetDrOriginalImageData();

	//int dim[3];
	//m_spDisplayImageData->GetDimensions(dim);
	if (!HasCTData())
	{
		return;
	}
	int size = m_w * m_h;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	heavyThrough(m_spOriginalDataPixel.get(), 2000, 10, m_w, m_h, tempData.get());
	CTColor((const unsigned short*)tempData.get(), m_w, m_h, m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
	FitWindow();
}

//**************************************************
//	brief   : DR图像增加多重吸收率
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::IncreaseMultiAbsorb()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::IncreaseMultiAbsorb");
	if (!HasDRData())
	{
		return;
	}
	m_spDisplayImageData = m_pDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int frame = 2; // 当前DR图像需要两桢计算
	int size = dim[0] * dim[1]*frame;
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(), data, size * sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	int max = INT_MIN;
	int min = INT_MAX;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			if (*data > max)
			{
				max = *data;
			}

			if (*data < min)
			{
				min = *data;
			}

			++data;
		}
	}

	data = (short*)m_spDisplayImageData->GetScalarPointer();
	short* temp = tempData.get();
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			*temp = (*data - min) * 5120 / (max - min);

			++temp;
			++data;
		}
	}

	std::map<short, short> absorb;
	for (int i = 0; i < 1000; ++i)
	{
		absorb[i] = i;
	}
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[m_w*m_h], std::default_delete<unsigned char[]>());
	adjustMultiAbsorb((unsigned short*)tempData.get(), m_w, m_h, absorb, spDes);
	RefreshImage(spDes.get(), dim[0], dim[1]);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : DR图像降低多重吸收率
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::DecreaseMultiAbsorb()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::DecreaseMultiAbsorb");
	if (!HasDRData())
	{
		return;
	}
	m_spDisplayImageData = m_pDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int frame = 2; // 当前DR图像需要两桢计算
	int size = dim[0] * dim[1]*frame;
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(), data, size * sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	int max = INT_MIN;
	int min = INT_MAX;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			if (*data > max)
			{
				max = *data;
			}

			if (*data < min)
			{
				min = *data;
			}

			++data;
		}
	}

	data = (short*)m_spDisplayImageData->GetScalarPointer();
	short* temp = tempData.get();
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			*temp = (*data - min) * 5120 / (max - min);

			++temp;
			++data;
		}
	}

	std::map<short, short> absorb;
	for (int i = 500; i < 1500; ++i)
	{
		absorb[i] = i;
	}
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[m_w*m_h], std::default_delete<unsigned char[]>());
	adjustMultiAbsorb((unsigned short*)tempData.get(), m_w, m_h, absorb, spDes);
	RefreshImage(spDes.get(), dim[0], dim[1]);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : DR图像低穿透处理
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::LightThroughDR()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::LightThroughDR");
	if (!HasDRData())
	{
		return;
	}
	m_spDisplayImageData = m_pDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int frame = 2;
	int size = dim[0] * dim[1]*frame;
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(), data, size * sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	int max = INT_MIN;
	int min = INT_MAX;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			if (*data > max)
			{
				max = *data;
			}

			if (*data < min)
			{
				min = *data;
			}

			++data;
		}
	}

	data = (short*)m_spDisplayImageData->GetScalarPointer();
	short* temp = tempData.get();
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			*temp = (*data - min) * 5120 / (max - min);

			++temp;
			++data;
		}
	}

	heavyThrough(tempData.get(), 3000, 3000, m_w, m_h, tempData.get());
	CTColor((unsigned short*)tempData.get(), dim[0], dim[1], m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), dim[0], dim[1]);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : DR图像高穿透处理
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::HavyThroughDR()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::HavyThroughDR");
	if (!HasDRData())
	{
		return;
	}
	auto spDataModule = m_pDataModule;
	m_spDisplayImageData = spDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int frame = 2;
	int size = dim[0] * dim[1]*frame;
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(), data, size * sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	int max = INT_MIN;
	int min = INT_MAX;
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	for (int j = 0; j < m_h;++j)
	{
		for (int i = 0; i < m_w;++i)
		{
			if (*data > max)
			{
				max = *data;
			}

			if (*data < min)
			{
				min = *data;
			}

			++data;
		}
	}

	data = (short*)m_spDisplayImageData->GetScalarPointer();
	short* temp = tempData.get();
	for (int j = 0; j < m_h; ++j)
	{
		for (int i = 0; i < m_w; ++i)
		{
			*temp = (*data - min) * 5120 / (max - min);

			++temp;
			++data;
		}
	}

	heavyThrough(tempData.get(),2000, 50, m_w, m_h, tempData.get());
	CTColor((unsigned short*)tempData.get(), dim[0], dim[1], m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), dim[0], dim[1]);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : CT图像灰度图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::PageGray()
{
	if (m_iIndex < 0 || m_eOrg==UNKNOWN)
	{
		LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::PageGray error !");
		return;
	}

	if (m_eOrg==TRA)
	{
		PageTraGray(m_iIndex);
	}
	else
	{
		PageSagGray(m_iIndex);
	}
}

//**************************************************
//	brief   : CT图像切片彩色图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::Page()
{
	if (m_eOrg == TRA)
	{
		PageTra(m_iIndex);
	}
	else
	{
		PageSag(m_iIndex);
	}
}

//**************************************************
//	brief   : CT图像横断位切片灰度图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::PageTraGray(int iIndex)
{
	if (m_spDisplayImageData == nullptr)
	{
		return;
	}
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::PageTraGray");
	m_spOriginalDataPixel = m_pDataModule->GetTraData(iIndex, m_w, m_h);
	m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
	m_spDisplayImageData->SetDimensions(m_w, m_h, 1);
	uchar ucChannel = 1;
	m_spDisplayImageData->SetScalarType(VTK_SHORT, info);

	double dSpacing[3];
	auto spImageData = m_pDataModule->GetOriginalImageData();
	spImageData->GetSpacing(dSpacing);
	m_spDisplayImageData->SetSpacing(dSpacing);

	m_spDisplayImageData->SetNumberOfScalarComponents(ucChannel, info);//每个像素需要表示的组份 =1是指标量图
	m_spDisplayImageData->AllocateScalars(info);//很重要

	auto pDesData = m_spDisplayImageData->GetScalarPointer();
	auto pSorData = m_spOriginalDataPixel.get();
	auto totalSize = m_w * m_h * sizeof(short);
	memcpy(pDesData, pSorData, totalSize);

	CTColor((unsigned short*)m_spOriginalDataPixel.get(), m_w, m_h, m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : CT图像横断位切片彩色图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::PageTra(int iIndex)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::PageTra");

	if (iIndex < 0)
	{
		return;
	}

	if (!HasCTData())
	{
		return;
	}

	m_iIndex = iIndex;
	m_eOrg = TRA;
	if (m_pDataModule->HasAlgo(Algorithm::Algo_Color))
	{
		m_spOriginalDataPixel = m_pDataModule->GetTraData(iIndex, m_w, m_h);
		//应用伪彩算法
		//color(m_spOriginalDataPixel.get(), m_w, m_h, m_spDisplayDataPixel);
		CTColor((const unsigned short*)m_spOriginalDataPixel.get(), m_w, m_h, m_spDisplayDataPixel);

		RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
		FitWindow();
	}
	else
	{
		PageTraGray(m_iIndex);
	}
    
}

//**************************************************
//	brief   : CT图像矢状位切片灰度图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::PageSagGray(int iIndex)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::PageSagGray"); 
	if (!HasCTData())
	{
		return;
	}
	m_spOriginalDataPixel = m_pDataModule->GetSagData(iIndex, m_w, m_h);
	m_spDisplayImageData = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
	m_spDisplayImageData->SetDimensions(m_w, m_h, 1);
	uchar ucChannel = 1;
	m_spDisplayImageData->SetScalarType(VTK_SHORT, info);

	double dSpacing[3];
	auto spImageData = m_pDataModule->GetOriginalImageData();
	spImageData->GetSpacing(dSpacing);
	double spacing[3];
	spacing[0] = dSpacing[1];
	spacing[1] = dSpacing[2];
	spacing[2] = dSpacing[0];
	m_spDisplayImageData->SetSpacing(spacing);

	m_spDisplayImageData->SetNumberOfScalarComponents(ucChannel, info);//每个像素需要表示的组份 =1是指标量图
	m_spDisplayImageData->AllocateScalars(info);//很重要

	auto pDesData = m_spDisplayImageData->GetScalarPointer();
	auto pSorData = m_spOriginalDataPixel.get();
	auto totalSize = m_w * m_h * sizeof(short);
	memcpy(pDesData, pSorData, totalSize);

	CTGray((unsigned short*)m_spOriginalDataPixel.get(), m_w, m_h, m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
	//m_spImageViewer->SetInputData(m_spDisplayImageData);
	//m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : CT图像矢状位切片彩色图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::PageSag(int iIndex)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::PageSag");
	if (iIndex < 0)
	{
		return;
	}
	if (!HasCTData())
	{
		return;
	}
	m_iIndex = iIndex;
	m_eOrg = SAG;
	if (m_pDataModule->HasAlgo(Algorithm::Algo_Color))
	{
		m_spOriginalDataPixel = m_pDataModule->GetSagData(iIndex, m_w, m_h);
		//应用伪彩算法
		CTColor((unsigned short*)m_spOriginalDataPixel.get(), m_w, m_h, m_spDisplayDataPixel);
		RefreshImage(m_spDisplayDataPixel.get(), m_w, m_h);
		FitWindow();
	}
	else
	{
		PageSagGray(m_iIndex);
	}
	 
}

void Cell2D::FitWindow()
{   
    //m_spImageViewer->Render();
    //return;
    if (nullptr == m_spDisplayImageData)
    {
        return;
    }
    m_spImageViewer->UpdateDisplayExtent();


    //auto size = m_pMainWnd->frameGeometry();
    auto *pVtkSize = m_spImageViewer->GetSize();
    int w = pVtkSize[0];
    int h = pVtkSize[1];

    auto spRender = m_spImageViewer->GetRenderer();
    vtkCamera* pCamera = spRender->GetActiveCamera();
    int extent[6];
    m_spDisplayImageData->GetExtent(extent);

    double origin[3];
    m_spDisplayImageData->GetOrigin(origin);

    double spacing[3];
    m_spDisplayImageData->GetSpacing(spacing);

    float xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
    float yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
    //  float xd = (extent[1] - extent[0] + 1)*spacing[0]; // not used
    float yd_y = (extent[3] - extent[2] + 1)*spacing[1];
    float xd = (extent[1] - extent[0] + 1)*spacing[0];

    //if (yd < xd)
    //{
    //    //vtkCamera::ParallelProjectionOn()开启平行／正交投影。此时需通过SetParallelScale()方法控制角色物体的缩放。Zoom不再有效。
    //    //yd = yd * h / w;
    //    yd = xd * h / w;
    //    pCamera->SetParallelScale(0.5f*static_cast<float>(yd));
    //}
    //else
    //{
    //    pCamera->SetParallelScale(0.5f*static_cast<float>(yd));
    //}     


    auto yd_x = xd * h / w;  //计算充满X轴时候的高度值
    auto yd = std::max(yd_y, yd_x);//计算充满Y轴时候的高度值
    pCamera->SetParallelScale(0.5f*static_cast<float>(yd));
    auto scale2 = pCamera->GetParallelScale();

    float d = pCamera->GetDistance();
    pCamera->SetFocalPoint(xc, yc, 0.0);
    pCamera->SetPosition(xc, yc, +d);
    UpdateRuler();
    m_spImageViewer->Render();

}

std::shared_ptr<unsigned char> Cell2D::GetDisplayDataPixel()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::GetDataPixel");
    return m_spDisplayDataPixel;
}

//**************************************************
//	brief   : 获取当前cell中原始图数据（未经过算法处理的）
//	in      :
//	out     :
//	return  : 返回数据
//**************************************************
std::shared_ptr<short> Cell2D::GetOriginalDataPixel()
{
    return m_spOriginalDataPixel;
}
void Cell2D::GetDimention(int &w, int &h)
{    
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::GetDimention");
    w = m_w;
    h = m_h;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Cell2D::GetDimention");
}

//**************************************************
//	brief   : DR图像灰度图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::ShowDRGray()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::ShowDRGray");
	if (!HasDRData())
	{
		return;
	}
	m_spDisplayImageData = m_pDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int size = dim[0] * dim[1] * dim[2];
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(), data, size * sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	DRGray((unsigned short*)m_spDisplayImageData->GetScalarPointer(), dim[0], dim[1], m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), dim[0], dim[1]);
	m_spImageViewer->SetInputData(m_spDisplayImageData);
	m_spImageViewer->Render();
	FitWindow();
}

//**************************************************
//	brief   : DR图像彩色图
//	in      :
//	out     :
//	return  : 
//**************************************************
void Cell2D::ShowDR()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Cell2D::ShowDR"); 
	if (!HasDRData())
	{
		return;
	}
	m_spDisplayImageData = m_pDataModule->GetDrOriginalImageData();

	int dim[3];
	m_spDisplayImageData->GetDimensions(dim);
	int size = dim[0] *dim[1]*dim[2];
	m_spOriginalDataPixel = std::shared_ptr<short>(new short[size], std::default_delete<short[]>());
	short* data = (short*)m_spDisplayImageData->GetScalarPointer();
	memcpy(m_spOriginalDataPixel.get(),data,size*sizeof(short));
	m_w = dim[0];
	m_h = dim[1];

	DRColor((unsigned short*)m_spDisplayImageData->GetScalarPointer(), dim[0], dim[1], m_spDisplayDataPixel);
	RefreshImage(m_spDisplayDataPixel.get(), dim[0], dim[1]);
	m_spImageViewer->SetInputData(m_spDisplayImageData);
	m_spImageViewer->Render();
	FitWindow();
}

void Cell2D::LoadPng(const string & sPath)
{
    vtkPNGReader* pngReader = vtkPNGReader::New(); 
    pngReader->SetFileName(sPath.c_str());//读取图片
    pngReader->Update();
    m_spDisplayImageData = pngReader->GetOutput();


    m_spImageViewer->SetInputConnection(pngReader->GetOutputPort());
    m_spImageViewer->Render();
    FitWindow();
}
/*-----------------------------------------------------------
-***   Description:     创建按钮
-***   OUTPUT     :
-***   INPUT      :
-------------------------------------------------------------*/

short Cell2D::GetPixelVaue(int pos[3])
{
    int x = pos[0];
    int y = pos[1];
    int iDim[3];
    m_spDisplayImageData->GetDimensions(iDim);
    if (x >= iDim[0] || y >= iDim[1])
    {
        return 0;
    }
    int iPos = y * iDim[0] + x;
    auto pData = static_cast<short*>(m_spDisplayImageData->GetScalarPointer());

    return *(pData + iPos);
}


