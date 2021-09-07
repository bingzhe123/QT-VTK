#include "DataModule.h"
#include "IPSImageReslice.h"

#include "vtkInformation.h"

/*-----------------------------------------------------------
***   Description:     设置数据包
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void DataModule::SetRawDataPacket(vtkSmartPointer<vtkImageData> spImageData)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DataModule::SetDataPacket::Begin");
    m_spOriginalImageData = spImageData;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DataModule::SetDataPacket::End");
}

/*-----------------------------------------------------------
***   Description:     设置数据包
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void DataModule::SetDrRawDataPacket(vtkSmartPointer<vtkImageData> spImageData)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "DataModule::SetDrDataPacket::Begin");
	m_spDrOriginalImageData = spImageData;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "DataModule::SetDrDataPacket::End");
}

/*-----------------------------------------------------------
***   Description:     vtk获取图片数据
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
vtkSmartPointer<vtkImageData> DataModule::GetOriginalImageData()
{
    return m_spOriginalImageData;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DataModule::GetImageData");
}

/*-----------------------------------------------------------
***   Description :     vtk获取Dr图片数据
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
vtkSmartPointer<vtkImageData> DataModule::GetDrOriginalImageData()
{
	return m_spDrOriginalImageData;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "DataModule::GetDrImageData");
}

/*-----------------------------------------------------------
***   Description:     获取sag数据
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
std::shared_ptr<short> DataModule::GetSagData(int index, int &w, int &h)
{
    auto pData = static_cast<short*>(m_spOriginalImageData->GetScalarPointer());
    int dim[3];
    m_spOriginalImageData->GetDimensions(dim);
    ReSlice slice(pData, dim[0], dim[1], dim[2]);
    std::shared_ptr<short> spOut;
    slice.getSagData(spOut, index,w,h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DataModule::GetSagData");
    return spOut;
}

/*-----------------------------------------------------------
***   Description:     获取tra数据
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
std::shared_ptr<short> DataModule::GetTraData(int index, int &w, int &h)
{
    auto pData = static_cast<short*>(m_spOriginalImageData->GetScalarPointer());
    //m_spImageData->GetScalarPointerForExtent();
    int dim[3];
    m_spOriginalImageData->GetDimensions(dim);
    ReSlice slice(pData, dim[0], dim[1], dim[2]);
    std::shared_ptr<short> spOut;
    slice.getTraData(spOut, index,w ,h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "DataModule::GetTraData");
    return spOut;
}

std::shared_ptr<short> DataModule::GetSubVolume(int bounds[6], vtkSmartPointer<vtkImageData> spImageData)
{
    auto pData = static_cast<short*>(spImageData->GetScalarPointer());
    int dim[3];
    spImageData->GetDimensions(dim);
    ReSlice slice(pData, dim[0], dim[1], dim[2]);
    std::shared_ptr<short> spOut;
    slice.getSubVolume(spOut, bounds);
    return spOut;
}

void DataModule::ClearData()
{
	m_spDrOriginalImageData = nullptr;
	m_spOriginalImageData = nullptr;
}

int DataModule::GetCurrentAlgo()
{
	return m_iCurrentAlgo;
}

void DataModule::SetAlgo(Algorithm al)
{
	m_iCurrentAlgo |= (int)al;
}

void DataModule::RemoveAlgo(Algorithm al)
{
	m_iCurrentAlgo &= ~((int)al);
}

bool DataModule::HasAlgo(Algorithm al)
{
	return m_iCurrentAlgo&(int)al;
}

void DataModule::ModifyDrRawData(vtkSmartPointer<vtkImageData> src,
	vtkSmartPointer<vtkImageData>& dest)
{
	int dim[3];
	src->GetDimensions(dim);

	int temp = dim[1];
	dim[1] = dim[0];
	dim[0] = temp;

	dest = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
	dest->SetDimensions(dim[0], dim[1], dim[2]);
	int ucChannel = 1;
	dest->SetScalarType(VTK_SHORT, info);

	double dSpacing[3];
	src->GetSpacing(dSpacing);

	double spac = dSpacing[1];
	dSpacing[1] = dSpacing[0];
	dSpacing[0] = spac;
	dest->SetSpacing(dSpacing);

	dest->SetNumberOfScalarComponents(ucChannel, info);//每个像素需要表示的组份 =1是指标量图
	dest->AllocateScalars(info);//很重要

	short* destData = (short*)dest->GetScalarPointer();
	short* srcData = (short*)src->GetScalarPointer();
	for (int k = 0; k < dim[2];++k)
	{
		for (int i = 0; i < dim[1]; ++i)
		{
			for (int j = dim[0]-1; j >= 0;--j)
			{
				*destData = srcData[k * dim[0] * dim[1] + j * dim[1] + i];

				++destData;
			}
		}
	}
	
	dest->UpdatePointGhostArrayCache();
	dest->UpdateCellGhostArrayCache();
}
