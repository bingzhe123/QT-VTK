/*----------------------------------------------------------------------------
   Component       : IPS - DATA
   Name            :
   Author          :
   Creation Date   : 07.MAY 2021
   Description     : json�ַ�������������
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  DATAMODULE_H_
#define  DATAMODULE_H_
#include "ModuleInterface.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

#include "gespips.h"
enum class Algorithm
{
	Algo_None = 0x00000000,
	Algo_Color = 0x00000001,//α��
};

class DataModule :
    public IModule
{
public:

    //**************************************************
    //	brief   : ��������
    //	in      :spImageData�����㷨�ж�ȡ������
    //	out     :
    //	return  : 
    //**************************************************
    void SetRawDataPacket(vtkSmartPointer<vtkImageData> spImageData);

	//**************************************************
	//	brief   : ����DR����
	//	in      : spImageData�����㷨�ж�ȡ������
	//	out     :
	//	return  : 
	//**************************************************
	void SetDrRawDataPacket(vtkSmartPointer<vtkImageData> spImageData);

    //**************************************************
    //	brief   : 
    //	in      :
    //	out     :
    //	return  :���ص�ǰԭʼ������
    //**************************************************
    vtkSmartPointer<vtkImageData> GetOriginalImageData();

	//**************************************************
	//	brief   : 
	//	in      :
	//	out     :
	//	return  :���ص�ǰԭʼ������
	//**************************************************
	vtkSmartPointer<vtkImageData> GetDrOriginalImageData();

    //**************************************************
    //	brief   :��ȡʸ״λ����Ƭͼ 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<short> GetSagData(int index, int &w, int &h);

    //**************************************************
    //	brief   :��ȡ���λ����Ƭ 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<short> GetTraData(int index, int &w, int &h);


    /*-----------------------------------------------------------
    ***   Description: ��ȡ�ü���������
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    std::shared_ptr<short> GetSubVolume(int bounds[6], vtkSmartPointer<vtkImageData>);

	/*-----------------------------------------------------------
	***   Description:�������
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ClearData();


	/*-----------------------------------------------------------
	***   Description:��ȡ��ǰӦ�õ��㷨maskֵ
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	int GetCurrentAlgo();

	/*-----------------------------------------------------------
	***   Description:��¼ʹ��ĳ���㷨
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetAlgo(Algorithm al);

	/*-----------------------------------------------------------
	***   Description:�Ƴ�ĳ���㷨�ļ�¼
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RemoveAlgo(Algorithm al);

	/*-----------------------------------------------------------
	***   Description:�Ƿ�ʹ����ĳ���㷨
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	bool HasAlgo(Algorithm al);

	//**************************************************
	//	brief   : �޸�DR����
	//	in      : src ����dicom�ļ��ж�ȡ������
	//	out     : dest : ԭʼ��������ת����õ�������
	//	return  : 
	//**************************************************
	void ModifyDrRawData(vtkSmartPointer<vtkImageData> src, vtkSmartPointer<vtkImageData>& dest);
private:

    vtkSmartPointer<vtkImageData> m_spOriginalImageData = nullptr;
	vtkSmartPointer<vtkImageData> m_spDrOriginalImageData = nullptr;

	int m_iCurrentAlgo = 1;//Ĭ��α���㷨
};
#endif // filedef !                 

