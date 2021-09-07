#pragma once

#include <QWidget>
#include "ui_SCSStateBar.h"

class SCSBarWidget : public QWidget
{
    Q_OBJECT

public:
    SCSBarWidget(QWidget *parent = Q_NULLPTR);
    ~SCSBarWidget();

private:
    Ui::SCSStateBar ui;
};
