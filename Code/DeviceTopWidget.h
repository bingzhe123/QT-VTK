#pragma once

#include <QtWidgets/QWidget>
#include "ui_DeviceTopWidget.h"

class DeviceTopWidget : public QWidget
{
    Q_OBJECT

public:
    DeviceTopWidget(QWidget *parent = Q_NULLPTR);
	Ui::DeviceTopWidgetClass ui;
private:
    
};
