/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 22. APR 2021
   Description     : 存储所有的module，widget, 在程序启动的时候初始化
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  MODULECONTAINER
#define  MODULECONTAINER

#include "commondef.h"
#include "ModuleInterface.h"
#include <share.h>
#include <vector>
#include "gespips.h"


class QWidget;
class Cell;


class Container
{
public:
    Container();
    ~Container();

    //**************************************************
    //	brief   :  添加module
    //	in      :  pIModule:需要添加的module
    //	out     :
    //	return  : 
    //**************************************************

    void AddModule(const string &sName, IModule *pModule);

public:
    //**************************************************
    //	brief   :获取module
    //	in      :
    //	out     :
    //	return  :返回指定类型的module
    //**************************************************
    template<typename T>
    T* GetModule(const string & sMoule)
    {
        if (m_mapModule.find(sMoule) != m_mapModule.end())
        {
            return dynamic_cast<T*>(m_mapModule[sMoule]);
        }
        return nullptr;
    }
    //**************************************************
    //	brief   :  添加所有窗口对象
    //	in      :  sName:窗口的名字
    //	out     :
    //	return  : 
    //**************************************************
    void AddWidget(const string &sName, QWidget *pWidget);

    //**************************************************
    //	brief   : 获取指定窗口对象
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    template<typename T>
    T* GetWidget(const string &sName)
    {
        if (m_mapWidget.find(sName) != m_mapWidget.end())
        {
            return dynamic_cast<T*>(m_mapWidget[sName]);
        }
        return nullptr;
    }

    //**************************************************
    //	brief   : 获取所有的cell
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    std::vector<Cell*> GetAllCell();

private:
    map<string, IModule*> m_mapModule;
    map<string, QWidget*> m_mapWidget;
};
using ContainerPtr = std::shared_ptr<Container>;

#endif // MOUDLECONTAINER !    
