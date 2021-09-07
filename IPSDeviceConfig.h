#pragma once

#include <QtWidgets/QWidget>
#include "ui_IPSDeviceConfig.h"

class IPSDeviceConfig : public QWidget
{
    Q_OBJECT

public:
    IPSDeviceConfig(QWidget *parent = Q_NULLPTR);

private:
    Ui::IPSDeviceConfigClass ui;
	void GetScreenShotDir();
	void GetScreenCapDir();
	QString GetDir(QString tips, QString dir);
};
