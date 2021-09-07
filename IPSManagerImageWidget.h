/**************************************************************************
Copyright: N.A

Author: N.A

Date:

Description:
**************************************************************************/
#ifndef  filedef
#define  filedef

#include <QWidget>
#include "BaseView.h"
#include "ui_IPSManagerImageWidget.h"

class IPSManagerImageWidget : public BaseView
{
    Q_OBJECT

public:
    IPSManagerImageWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~IPSManagerImageWidget();

private:
    Ui::IPSManagerImageWidget ui;
};




#endif // filedef !   

