#pragma once

#include <QtWidgets/QWidget>
#include "ui_IPSDeviceInformation.h"
#include <QLabel>
#include <QPainter>
#include "DeviceInformation.h"
#include "DeviceStatusWidget.h"
#include "DeviceTopWidget.h"
#include "IPSDeviceConfig.h"
#include "DeviceLog.h"
class IPSDeviceInformation : public QWidget
{
	Q_OBJECT

public:
	IPSDeviceInformation(QWidget *parent = Q_NULLPTR);
private:
	Ui::IPSDeviceInformationClass ui;
	void paintEvent(QPaintEvent*event);
	void ChangeView(int View, int AimView);
	DeviceTopWidget*ksTopWidget;
	DeviceStatusWidget*ksStatusWidget;
	DeviceInformation*ksInformationWidget;
	IPSDeviceConfig*ksConfigWidget;
	DeviceLog*ksLogWidget;
};
