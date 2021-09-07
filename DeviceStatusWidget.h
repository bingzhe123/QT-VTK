#pragma once

#include <QtWidgets/QWidget>
#include "ui_DeviceStatusWidget.h"

class DeviceStatusWidget : public QWidget
{
    Q_OBJECT

public:
    DeviceStatusWidget(QWidget *parent = Q_NULLPTR);
	int viewSign;
private:
    Ui::DeviceStatusWidgetClass ui;
	void ChangeInfo();
	void ChangeConfig();
	void ChangeLog();

signals:
	void ChangeView(int,int);

};
