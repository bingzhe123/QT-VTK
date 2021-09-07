#pragma once

#include <QtWidgets/QWidget>
#include <QSettings>
#include <QSysinfo>
#include "QSysinfo"
#include "ui_DeviceInformation.h"

class DeviceInformation : public QWidget
{
    Q_OBJECT

public:
    DeviceInformation(QWidget *parent = Q_NULLPTR);

private:
    Ui::DeviceInformationClass ui;

};
