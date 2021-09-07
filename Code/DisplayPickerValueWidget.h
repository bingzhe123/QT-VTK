/*----------------------------------------------------------------------------
Component       : IPS - View
Name            : DisplayPickerValueWidget
Author          :
Creation Date   : 2021.05.30
Description     : 切片窗口显示当前鼠标位置的原子序数和密度
Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef DISPLAYPICKERVALUEWIDGET_H_
#define DISPLAYPICKERVALUEWIDGET_H_

#include <QWidget>
#include <qmainwindow.h>
#include "ui_DisplayPickerValueWidget.h"
#include "BaseView.h"


class DisplayPickerValueWidget : public BaseView
{
    Q_OBJECT

public:
    DisplayPickerValueWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);

    ~DisplayPickerValueWidget();


    //**************************************************
    //	brief   :更新统计信息 
    //	in      :当前选择的点信息
    //	out     :
    //	return  : 
    //**************************************************
    void UpdateStatisticInfo(const vector<short> &&vecPixel);

private:
    Ui::DisplayPickerValueWidget ui;
};

#endif // !DISPLAYPICKERVALUEWIDGET_H_

