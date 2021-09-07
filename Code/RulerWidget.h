#pragma once

#include <QWidget>
#include "ui_RulerWidget.h"
#include "BaseView.h"

class RulerWidget : public BaseView
{
    Q_OBJECT

public:
    RulerWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~RulerWidget();

    void paintEvent(QPaintEvent *pEvent);

private:
    Ui::RulerWidget ui;
};
