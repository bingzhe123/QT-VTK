/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 
   Description     : CT ”Õº
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CTVIEWWIDGETS_H_
#define  CTVIEWWIDGETS_H_

#include "ui_CTViewWidgets.h"
#include "BaseView.h"
#include <QSlider>
class CTViewWidgets : public BaseView
{
    Q_OBJECT

public:
    CTViewWidgets(ContainerPtr spContainer, QWidget *parent = Q_NULLPTR);
    ~CTViewWidgets();
    virtual void AttachView();
public slots:
	void OnHorizontalSlideChange(int iPos);
	void OnVerticalSliderChange(int iPos);
public:
	void DisableVerti();
	void SwitchHoriOrVerti(bool a);
	QSlider*verti, *hori;
private:
    Ui::CTViewWidgets ui;
	
};





#endif // CTVIEWWIDGETS_H_ !   

