/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 22. APR 2021
   Description     : �洢���е�module��widget, �ڳ���������ʱ���ʼ��
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
    //	brief   :  ���module
    //	in      :  pIModule:��Ҫ��ӵ�module
    //	out     :
    //	return  : 
    //**************************************************

    void AddModule(const string &sName, IModule *pModule);

public:
    //**************************************************
    //	brief   :��ȡmodule
    //	in      :
    //	out     :
    //	return  :����ָ�����͵�module
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
    //	brief   :  ������д��ڶ���
    //	in      :  sName:���ڵ�����
    //	out     :
    //	return  : 
    //**************************************************
    void AddWidget(const string &sName, QWidget *pWidget);

    //**************************************************
    //	brief   : ��ȡָ�����ڶ���
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
    //	brief   : ��ȡ���е�cell
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
