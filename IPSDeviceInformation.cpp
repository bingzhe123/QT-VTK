#include "IPSDeviceInformation.h"

IPSDeviceInformation::IPSDeviceInformation(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.widget->raise();
	ksTopWidget = new DeviceTopWidget(this);
	ksStatusWidget = new DeviceStatusWidget(this);
	ksInformationWidget = new DeviceInformation(this);
	ksConfigWidget = new IPSDeviceConfig(this);
	ksLogWidget = new DeviceLog(this);
	ksConfigWidget->hide();
	ksLogWidget->hide();
	ui.hori->addWidget(ksTopWidget);
	ui.status->addWidget(ksStatusWidget);
	ui.info->addWidget(ksInformationWidget);
	connect(ksStatusWidget, &DeviceStatusWidget::ChangeView, this,&IPSDeviceInformation::ChangeView);
	
	connect(ksTopWidget->ui.pushButton, &QPushButton::clicked, this, &IPSDeviceInformation::hide);
}

void IPSDeviceInformation::paintEvent(QPaintEvent * event)
{
	//LOG_DEBUG(GESP::DebugLogger::GetInstance(), "TopStateBarWidget::paintEvent");
	auto r = rect();
	r.setHeight(320);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QLinearGradient linearGradient(r.left(), r.top(), r.right(), r.bottom());
	linearGradient.setColorAt(0, "#854ECA");
	linearGradient.setColorAt(1.0, "#2080FF");
	painter.setBrush(QBrush(linearGradient));
	painter.fillRect(r, QBrush(linearGradient));
	//LOG_DEBUG(GESP::DebugLogger::GetInstance(), "TopStateBarWidget::paintEvent");
}

void IPSDeviceInformation::ChangeView(int View, int AimView)
{
	switch (View)
	{
	case 1:ui.info->removeWidget(ksInformationWidget); ksInformationWidget->hide(); break;
	case 2:ui.info->removeWidget(ksConfigWidget); ksConfigWidget->hide(); break;
	case 3:ui.info->removeWidget(ksLogWidget); ksLogWidget->hide(); break;
	}
	switch (AimView)
	{
	case 1:ui.info->addWidget(ksInformationWidget); ksInformationWidget->show(); break;
	case 2:ui.info->addWidget(ksConfigWidget); ksConfigWidget->show(); break;
	case 3:ui.info->addWidget(ksLogWidget); ksLogWidget->show(); break;
	}
}