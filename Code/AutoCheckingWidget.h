#pragma once

#include <QWidget>
#include "ui_AutoCheckingWidget.h"
#include "BaseView.h"

class AutoCheckingWidget : public QWidget,public BaseView
{
    Q_OBJECT

public:
    AutoCheckingWidget(QWidget *parent = Q_NULLPTR);
    ~AutoCheckingWidget();
Q_SIGNALS:
    void ChangeView(View view);

private slots:
    void Test();


public:
    Ui::AutoCheckingWidget ui;
};
