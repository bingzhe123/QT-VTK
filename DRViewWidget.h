/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. APR 2021
   Description     : DRÊÓÍ¼´°¿Ú
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  DRVIEWWIDGET_H_
#define  DRVIEWWIDGET_H_
#include <QWidget>
#include "ui_DRViewWidget.h"
#include "BaseView.h"

class DRViewWidget : public BaseView
{
    Q_OBJECT

public:
    DRViewWidget(ContainerPtr spContainer, QWidget *parent = Q_NULLPTR);
    ~DRViewWidget();
    virtual void AttachView();

private:
    Ui::DRViewWidget ui;
};





#endif // DRVIEWWIDGET_H_ !   
