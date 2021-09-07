/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 22. APR 2021
   Description     : 存储所有的module，widget, 在程序启动的时候初始化
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  BASE_VIEW_H_
#define  BASE_VIEW_H_

#include "commondef.h"
#include "ModuleContainer.h"
#include <share.h>  
#include <QWidget>
#include "gespips.h"
class IPSAPP;
class BaseView :  public QWidget
{
    Q_OBJECT
public:
    BaseView(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    virtual void AttachView();
Q_SIGNALS:
    void ChangeView(View view);
    void PageTra(int iIndex);
    void PageSag(int iIndex);
protected:
    IPSAPP * m_pApp;
    std::shared_ptr< Container> m_spContainer;
};

#endif // BASE_VIEW_H_ !   
