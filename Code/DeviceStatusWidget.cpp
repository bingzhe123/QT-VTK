#include "DeviceStatusWidget.h"

DeviceStatusWidget::DeviceStatusWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.buttonDeviceInfo->setStyleSheet("QPushButton{background:white;color:#626262;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
	ui.buttonDeviceConfig->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
	ui.buttonDeviceLog->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
	ui.buttonDeviceInfo->setEnabled(false);
	ui.buttonDeviceConfig->setEnabled(true);
	ui.buttonDeviceLog->setEnabled(true);
	viewSign = 1;
	connect(ui.buttonDeviceInfo, &QPushButton::clicked, this, &DeviceStatusWidget::ChangeInfo);
	connect(ui.buttonDeviceConfig, &QPushButton::clicked, this, &DeviceStatusWidget::ChangeConfig);
	connect(ui.buttonDeviceLog, &QPushButton::clicked, this, &DeviceStatusWidget::ChangeLog);
}


void DeviceStatusWidget::ChangeInfo()
{
	int a = viewSign;
	if (viewSign != 1)
	{
		ui.buttonDeviceInfo->setStyleSheet("QPushButton{background:white;color:#626262;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceConfig->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceLog->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceInfo->setEnabled(false);
		ui.buttonDeviceConfig->setEnabled(true);
		ui.buttonDeviceLog->setEnabled(true);
		viewSign = 1;
		emit ChangeView(a, 1);
	}
}
void DeviceStatusWidget::ChangeConfig()
{
	int a = viewSign;
	if (viewSign != 2)
	{
		ui.buttonDeviceInfo->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceConfig->setStyleSheet("QPushButton{background:white;color:#626262;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceLog->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceInfo->setEnabled(true);
		ui.buttonDeviceConfig->setEnabled(false);
		ui.buttonDeviceLog->setEnabled(true);
		viewSign = 2;
		emit ChangeView(a, 2);
	}
}
void DeviceStatusWidget::ChangeLog()
{
	int a = viewSign;
	if (viewSign != 3)
	{
		ui.buttonDeviceInfo->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceConfig->setStyleSheet("QPushButton{background:#444444;color:white;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceLog->setStyleSheet("QPushButton{background:white;color:#626262;border-top-left-radius:8px;border-bottom-left-radius:8px;font:16px PingFang SC;border:0 solid grey;}");
		ui.buttonDeviceInfo->setEnabled(true);
		ui.buttonDeviceConfig->setEnabled(true);
		ui.buttonDeviceLog->setEnabled(false);
		viewSign = 3;
		emit ChangeView(a,3);
	}
}
