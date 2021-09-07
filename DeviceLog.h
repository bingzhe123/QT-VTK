#pragma once

#include <QtWidgets/QWidget>
#include "ui_DeviceLog.h"

class DeviceLog : public QWidget
{
    Q_OBJECT

public:
    DeviceLog(QWidget *parent = Q_NULLPTR);

private:
    Ui::DeviceLogClass ui;
};
