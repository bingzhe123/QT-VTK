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
    //	brief   : ������ʾ��ͼ��������ɺ��Զ�ˢ��
    //	in      : pData��������� 
    //	out     :
    //	return  : 
    //**************************************************
    void RefreshImage(uchar *pData, int w, int h);

    //**************************************************
    //	brief   :��ҳ 
    //	in      :���λ��ǰ����ʾ������λ��
    //	out     :
    //	return  : 
    //**************************************************
    void PageTra(int iIndex);

	//**************************************************
	//	brief   :��ҳ 
	//	in      :���λ��ǰ����ʾ������λ��
	//	out     :
	//	return  : 
	//**************************************************
	void PageTraGray(int iIndex);

	//**************************************************
	//	brief   : CT��ά�Ҷ�ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void PageGray();

	//**************************************************
	//	brief   : CT�ߴ�͸ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThroughCT();

	//**************************************************
	//	brief   : CT�ʹ�͸ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void LightThroughCT();

	//**************************************************
	//	brief   : DR�ߴ�͸ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void HavyThroughDR();

	//**************************************************
	//	brief   : DR�ʹ�͸ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void LightThroughDR();

	//**************************************************
	//	brief   : DR���Ͷ��������� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void DecreaseMultiAbsorb();

	//**************************************************
	//	brief   : DR���Ӷ��������� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void IncreaseMultiAbsorb();

	//**************************************************
	//	brief   : ��ʾ�ڰ���Ƭͼ��
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void Page();

    //**************************************************
    //	brief   :��ҳ 
    //	in      :ʸ״λ��ҳ
    //	out     :
    //	return  : 
    //**************************************************
    void PageSag(int iIndex);

	//**************************************************
	//	brief   : ��ҳ 
	//	in      : ʸ״λ��ҳ
	//	out     :
	//	return  : 
	//**************************************************
	void PageSagGray(int iIndex);

	//**************************************************
	//	brief   : ��ʾDR��ɫͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ShowDR();

	//**************************************************
	//	brief   : ��ʾDR�Ҷ�ͼ�� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ShowDRGray();

	/*-----------------------------------------------------------
	***   Description:����Ӧ����
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
    virtual void FitWindow();


    //**************************************************
    //	brief   : ��ȡ��ǰcell����ʾ������
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<unsigned char> GetDisplayDataPixel();


    //**************************************************
    //	brief   : ��ȡ��ǰcell��ԭʼͼ���ݣ�δ�����㷨����ģ�
    //	in      :
    //	out     :
    //	return  : ��������
    //**************************************************
    std::shared_ptr<short> GetOriginalDataPixel();

    void GetDimention(int &w, int&h);


    void LoadPng(const string &sPath);


    //**************************************************
    //	brief   : ��ȡָ��λ�õ�ͼ�������ֵ
    //	in      : pos��Ҫ��ȡ��λ��
    //	out     :
    //	return  : ָ��λ�õ�����ֵ 
    //**************************************************
    short GetPixelVaue(int pos[3]);  

    /*-----------------------------------------------------------
    ***   Description: ��ȡ��ǰcell�е�IamgeData
    ***   OUTPUT   :
    ***   INPUT   :
    ------------------------------------------------------------*/
    vtkSmartPointer<vtkImageData> GetImageData();
    /*-----------------------------------------------------------
    ***   Description:��ȡ�ڵ�ǰ��ʾ������ÿ�����ر�ʾ��������
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual double GetPixelSize();

	/*-----------------------------------------------------------
	***   Description:��ͼ���������ͼ����ʾ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	virtual void ClearIamge();

public:
    vtkSmartPointer<vtkImageViewer2> m_spImageViewer;

    vtkSmartPointer<CustomStyleImage> m_spStyleImage;
    //����������
    std::shared_ptr<ImportImagesData> m_spImportImagesData;

    //��ǰ��ʾ������
    std::shared_ptr<short> m_spOriginalDataPixel;
    std::shared_ptr<unsigned char> m_spDisplayDataPixel;

    //ͼ��Ĵ�С
    int m_w;
    int m_h; 

	int m_iIndex;  // ͼ������
	IMG_ORG m_eOrg;  // ͼ����
};


#endif // CELL2D_H_ !   


