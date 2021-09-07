/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 15. APR 2021
   Description     : 
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CELL2D_H_
#define  CELL2D_H_

#include "Cell.h"
#include "vtkImageData.h"
#include "IPSImportImagesData.h"
#include <share.h>
#include <memory>
#include "gespips.h"
#include "vtkCellPicker.h"
class CustomStyleImage;

enum IMG_ORG
{
	TRA,
	SAG,

	UNKNOWN
};

class Cell2D : public Cell
{
public:
    Cell2D(ContainerPtr spContainer, QWidget *pMainWnd);

    virtual ~Cell2D();

    virtual void Initialize();

    void SetImageViewer(vtkSmartPointer<vtkImageViewer2> spImageView);

    //**************************************************
    //	brief   : 设置显示的图像，设置完成后自动刷新
    //	in      : pData输入的数据 
    //	out     :
    //	return  : 
    //**************************************************
    void RefreshImage(uchar *pData, int w, int h);

    //**************************************************
    //	brief   :翻页 
    //	in      :横断位当前的显示的索引位置
    //	out     :
    //	return  : 
    //**************************************************
    void PageTra(int iIndex);

	//**************************************************
	//	brief   :翻页 
	//	in      :横断位当前的显示的索引位置
	//	out     :
	//	return  : 
	//**************************************************
	void PageTraGray(int iIndex);

	//**************************************************
	//	brief   : CT二维灰度图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void PageGray();

	//**************************************************
	//	brief   : CT高穿透图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThroughCT();

	//**************************************************
	//	brief   : CT低穿透图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void LightThroughCT();

	//**************************************************
	//	brief   : DR高穿透图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThroughDR();

	//**************************************************
	//	brief   : DR低穿透图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void LightThroughDR();

	//**************************************************
	//	brief   : DR降低多重吸收率 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void DecreaseMultiAbsorb();

	//**************************************************
	//	brief   : DR增加多重吸收率 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void IncreaseMultiAbsorb();

	//**************************************************
	//	brief   : 显示黑白切片图像
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void Page();

    //**************************************************
    //	brief   :翻页 
    //	in      :矢状位翻页
    //	out     :
    //	return  : 
    //**************************************************
    void PageSag(int iIndex);

	//**************************************************
	//	brief   : 翻页 
	//	in      : 矢状位翻页
	//	out     :
	//	return  : 
	//**************************************************
	void PageSagGray(int iIndex);

	//**************************************************
	//	brief   : 显示DR彩色图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ShowDR();

	//**************************************************
	//	brief   : 显示DR灰度图像 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ShowDRGray();

	/*-----------------------------------------------------------
	***   Description:自适应窗口
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
    virtual void FitWindow();


    //**************************************************
    //	brief   : 获取当前cell中显示的数据
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<unsigned char> GetDisplayDataPixel();


    //**************************************************
    //	brief   : 获取当前cell中原始图数据（未经过算法处理的）
    //	in      :
    //	out     :
    //	return  : 返回数据
    //**************************************************
    std::shared_ptr<short> GetOriginalDataPixel();

    void GetDimention(int &w, int&h);


    void LoadPng(const string &sPath);


    //**************************************************
    //	brief   : 获取指定位置的图像的像素值
    //	in      : pos需要获取的位置
    //	out     :
    //	return  : 指定位置的像素值 
    //**************************************************
    short GetPixelVaue(int pos[3]);  

    /*-----------------------------------------------------------
    ***   Description: 获取当前cell中的IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    vtkSmartPointer<vtkImageData> GetImageData();
    /*-----------------------------------------------------------
    ***   Description:获取在当前显示窗口下每个像素表示的物理长度
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize();

	/*-----------------------------------------------------------
	***   Description:判图结束后清除图像显示
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void ClearIamge();

public:
    vtkSmartPointer<vtkImageViewer2> m_spImageViewer;

    vtkSmartPointer<CustomStyleImage> m_spStyleImage;
    //导入数据类
    std::shared_ptr<ImportImagesData> m_spImportImagesData;

    //当前显示的数据
    std::shared_ptr<short> m_spOriginalDataPixel;
    std::shared_ptr<unsigned char> m_spDisplayDataPixel;

    //图像的大小
    int m_w;
    int m_h; 

	int m_iIndex;  // 图像索引
	IMG_ORG m_eOrg;  // 图像方向
};


#endif // CELL2D_H_ !   


