/**************************************************************************
Copyright: N.A

Author: N.A

Date:

Description:
**************************************************************************/

#ifndef  AUTOCHECKINGMODULE
#define  AUTOCHECKINGMODULE

#include <map>
#include <vector>
#include <string>
using std::vector;
using std::map;
using std::string;


/**************************************************************************
//Summary: 自检状态定义
//
//      
**************************************************************************/

enum class AutoCheckingItem
{
    Communication,
    Memory,
    Waring,
    PhotoelectricSwitch,//光电开关
    InterlockSwitch,//联锁开关
    EmergencySwitch,//紧急开关
    ConveyorBeltMoving,//传送带运动
    SlipRing,//滑环
    TrainingManagement,//训管         
    ConveyorBeltCleaning,//传送带运动
    DR,
    CT
};


 

/**************************************************************************
//Summary: 自检Module
// 记录自检的结果
//      
**************************************************************************/

class AutoCheckingModule
{
public:
    AutoCheckingModule();
    ~AutoCheckingModule();

    //**************************************************
    //	brief   :  增加某个类型的一条自检结果
    //	in      : eItem：自检条目    
    //	in      : eItem：这个条目的一条结果信息    
    //	out     :  N.A
    //	return  :  N.A
    //**************************************************     
    void AddAutoCheckingResult(AutoCheckingItem eItem, const string &sItem);


    //**************************************************
    //	brief   : 获取某条自检结果
    //	in      :
    //	out     :
    //	return  : 返回自检结果
    //**************************************************

    vector<string> GetAutoCheckingResult(AutoCheckingItem eItem);

private:

    map< AutoCheckingItem, vector<string>> m_mapAutoCheckingResult;
};



#endif // AUTOCHECKINGMODULE !   
