/*----------------------------------------------------------------------------
   Component       : IPS - DEFINE
   Name            :
   Author          :
   Creation Date   : 06.MAY 2021
   Description     : 
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#include <string>   
#include <map>         
#include <vector>
#include <QString>
#include "SuspiciousObject.h"
using std::vector;
using std::string;
using std::map;

#ifndef  COMMONDEF
#define  COMMONDEF
#define PER_DAY 86400
#define PER_HOUR 3600
#define PER_MINUTE 60
#define PER_SECOND 1
#define heightside 0.829
#define widthside 0.6972
enum class View
{
    None,
    MainView,
    DRVIEW,
    CTVIEW,
	ManageVIEW
};

//3d��ͼ��viewDirection
enum class ViewDirection
{
    FrontView,//ǰ��ͼ
    RearView,    //����ͼ
    TopView,    //����ͼ       
    BottomView, //����ͼ
    LeftView,   //����ͼ
    RightView,  //����ͼ
};
const string ksAppWidget = "IpsAppWidget";
const string ksAutoCheckingModule = "AutoCheckingModule";
const string ksAutoCheckingWidget = "AutoCheckingWidget";
const string ksScsBarWidget = "ScsBarWidget";
const string ksMainViewWidget = "MainViewWidget";
const string ksDRViewWidget = "DRViewWidget";
const string ksCTViewWidget = "CTViewWidget";

const string ksTopStatusWidget = "TopStatusWidget";
const string ksStatusWidget = "BottomStatusWidget";
const string ksResultWidget = "ResultWidget";
const string ksToolBarWidget = "ToolBarWidget";
const string ksCheckResultWidget = "CheckResultWidget";
const string ksDisplayPickerValueWidget = "DisplayPickerValueWidget";
const string ksIPSManagerImageWidget = "IPSManagerImageWidget";

//ͨ��Э���ַ���
const string ksSCSCommand[15] = { "IPS_Login","CT_Reconstruction_Complete","DR_Reconstruction_Complete","IPS_Login_out","Manual_Judgment","Manual_Judgment_Result","Modify_Model","Get_Review_List","Get_Session_Result","Get_All_Users","Delete_Picture","Lock_Unlock_Picture","Get_Image_Backtrack","Exit_Busy_Mode","Second_Judgment_Result" };
const string ksID = "session_ID";
const string ksReturn = "return";
const string ksSessionListSize = "session_list_size";
const string ksEID = "equip_id";
const string ksCommandIndex = "command_index";
const string ksPassword = "password";
const string ksName = "name";
const string ksAICount = "ai_counts";
const string ksCount = "counts";
const string ksAIResult = "ai_result";
const string ksStartTime = "start_time";
const string ksEndTime = "end_time";
const string ksUserName = "user_name";
const string ksLock = "is_lock";
const string ksListSize = "list_size";
const string ksStationType = "station_type";
const string ksPermission = "Permissions";
const string ksRecallNumber = "Backtrack_number";



const string ksImageManage = "7";		//ͼ�����
const string ksImageLockUnlock = "9";	//ͼ����������
const string ksDeviceInfo = "10";		//�豸��Ϣ
const string ksCheckStuff = "11";		//��ͼҵ��
const string ksPrintStuff = "12";		//��ӡ
const string ksDeleteImage = "13";		//ɾ��ͼ��
const string ksOverTime = "14";			//��ʱ�ǳ�


//���Ͻ���ʾ���ͼƬ����ԭʼͼ
const string ksOriginalCellWidget = "OriginalCellWidget";
//���½���ͼ����
const string ksSliceCellWidget = "SliceCellWidget";
//3D��ͼ����
const string ks3DCellWidget = "3DCellWidget";

//�ü���ͼ����
const string ksCroppintCellWidget = "CroppintCellWidget";

//datamodule
const string ksDataModule = "ksDataModule";


//���Ͻ���ʾ���ͼƬ����ԭʼͼ
const string ksIspManagerOriginalCellWidget = "IspManagerOriginalCellWidget";
//���½���ͼ����
const string ksIspManagerSliceCellWidget = "IspManagerSliceCellWidget";
//3D��ͼ����
const string ksIspManager3DCellWidget = "IspManager3DCellWidget";

//�ü���ͼ����
const string ksIspManagerCroppintCellWidget = "IspManagerCroppintCellWidget";


const string ksIspManagerResultWidget = "IspManagerResultWidget";
const string ksIspManagerCheckResultWidget = "IspManagerCheckResultWidget";
const string ksIspManagerToolBarWidget = "IspManagerToolBarWidget";
const string ksIspManagerDataModule = "ksIspManagerDataModule";

const string ksDownloadFold = "C:\\MoveDicom";

#define DR_IMAGE 1
#define CT_IMAGE 2
#define DR_CT_IMAGE 3

class Point3D
{
public:
    Point3D(int x, int y, int z) :
        m_x(x), m_y(y), m_z(z)
    {
    }
    Point3D(const Point3D &r)
    {
        m_x = r.m_x;
        m_y = r.m_y;
        m_z = r.m_z;
    }
    Point3D&operator=(const Point3D &r)
    {
        m_x = r.m_x;
        m_y = r.m_y;
        m_z = r.m_z;
        return *this;
    }
public:
    int m_x;
    int m_y;
    int m_z;
};

//����Action
enum class OperationAction
{
    ActionPointer = 0,//ָ��
    ActionRuler = 1,  //ֱ�߲���
    ActionCropping = 2 //�ü�
};
      
typedef struct TimeCount
{
	int day, hour, minute, second, ms;
}timeCount;

class Permission	//Ȩ��
{
public:
	bool ImageManage ;		//ͼ�����
	bool ImageLockUnlock;	//ͼ����������
	bool DeviceInfo;		//�豸��Ϣ
	bool CheckStuff;		//��ͼҵ��
	bool PrintStuff;		//��ӡ
	bool DeleteImage;		//ɾ��ͼ��
	bool OverTime;			//��ʱ�ǳ�
	Permission(std::string permissionString)
	{
		ImageManage = permissionString.find(ksImageManage) ? true : false;
		ImageLockUnlock = permissionString.find(ksImageLockUnlock) ? true : false;
		DeviceInfo = permissionString.find(ksDeviceInfo) ? true : false;
		CheckStuff = permissionString.find(ksCheckStuff) ? true : false;
		PrintStuff = permissionString.find(ksPrintStuff) ? true : false;
		DeleteImage = permissionString.find(ksDeleteImage) ? true : false;
		OverTime = permissionString.find(ksOverTime) ? true : false;
	}
	Permission();
};

/*-----------------------------------------------------------
***   Description:     �Զ���ʱ���ʽ
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
timeCount* translateTime(__int64 t);


/*-----------------------------------------------------------
***   Description:     ʱ���ʽ�����
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
QString TimeCountTOTimeLog(timeCount* a);

#endif // COMMONDEF !   
