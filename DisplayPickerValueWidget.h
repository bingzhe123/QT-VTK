/*----------------------------------------------------------------------------
Component       : IPS - View
Name            : DisplayPickerValueWidget
Author          :
Creation Date   : 2021.05.30
Description     : ��Ƭ������ʾ��ǰ���λ�õ�ԭ���������ܶ�
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
    //	brief   :����ͳ����Ϣ 
    //	in      :��ǰѡ��ĵ���Ϣ
    //	out     :
    //	return  : 
    //**************************************************
    void UpdateStatisticInfo(const vector<short> &&vecPixel);

private:
    Ui::DisplayPickerValueWidget ui;
};

#endif // !DISPLAYPICKERVALUEWIDGET_H_

