/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : 判图操作倒计时等
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#pragma once

#include <QWidget>
#include "ui_MainViewWidget.h"
#include "BaseView.h"
#include "IPSToolsBar.h"

class MainViewWidget : public BaseView
{
    Q_OBJECT

public:
    MainViewWidget(ContainerPtr spContainer, QWidget *parent = Q_NULLPTR);
    ~MainViewWidget();
    //**************************************************
    //	brief   :设置view中的控件 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void AttachView();

    void LoadData();

	QSlider *hori, *verti;
private slots:
    void OnHorizontalSlideChange(int iPos);
    void OnVerticalSliderChange(int iPos);
	
public:
    Ui::MainViewWidget ui;
    IPSToolsBar*toolbar;
	void DisableVerti();
};
