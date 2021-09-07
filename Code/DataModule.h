/*----------------------------------------------------------------------------
   Component       : IPS - DATA
   Name            :
   Author          :
   Creation Date   : 07.MAY 2021
   Description     : json字符串操作辅助类
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
	Algo_Color = 0x00000001,//伪彩
};

class DataModule :
    public IModule
{
public:

    //**************************************************
    //	brief   : 设置数据
    //	in      :spImageData：从算法中读取的数据
    //	out     :
    //	return  : 
    //**************************************************
    void SetRawDataPacket(vtkSmartPointer<vtkImageData> spImageData);

	//**************************************************
	//	brief   : 设置DR数据
	//	in      : spImageData：从算法中读取的数据
	//	out     :
	//	return  : 
	//**************************************************
	void SetDrRawDataPacket(vtkSmartPointer<vtkImageData> spImageData);

    //**************************************************
    //	brief   : 
    //	in      :
    //	out     :
    //	return  :返回当前原始的数据
    //**************************************************
    vtkSmartPointer<vtkImageData> GetOriginalImageData();

	//**************************************************
	//	brief   : 
	//	in      :
	//	out     :
	//	return  :返回当前原始的数据
	//**************************************************
	vtkSmartPointer<vtkImageData> GetDrOriginalImageData();

    //**************************************************
    //	brief   :获取矢状位的切片图 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<short> GetSagData(int index, int &w, int &h);

    //**************************************************
    //	brief   :获取横断位的切片 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::shared_ptr<short> GetTraData(int index, int &w, int &h);


    /*-----------------------------------------------------------
    ***   Description: 获取裁剪的体数据
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    std::shared_ptr<short> GetSubVolume(int bounds[6], vtkSmartPointer<vtkImageData>);

	/*-----------------------------------------------------------
	***   Description:清除数据
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ClearData();


	/*-----------------------------------------------------------
	***   Description:获取当前应用的算法mask值
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	int GetCurrentAlgo();

	/*-----------------------------------------------------------
	***   Description:记录使用某个算法
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetAlgo(Algorithm al);

	/*-----------------------------------------------------------
	***   Description:移除某个算法的记录
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void RemoveAlgo(Algorithm al);

	/*-----------------------------------------------------------
	***   Description:是否使用了某个算法
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	bool HasAlgo(Algorithm al);

	//**************************************************
	//	brief   : 修改DR数据
	//	in      : src ：从dicom文件中读取的数据
	//	out     : dest : 原始数据行列转换后得到的数据
	//	return  : 
	//**************************************************
	void ModifyDrRawData(vtkSmartPointer<vtkImageData> src, vtkSmartPointer<vtkImageData>& dest);
private:

    vtkSmartPointer<vtkImageData> m_spOriginalImageData = nullptr;
	vtkSmartPointer<vtkImageData> m_spDrOriginalImageData = nullptr;

	int m_iCurrentAlgo = 1;//默认伪彩算法
};
#endif // filedef !                 

