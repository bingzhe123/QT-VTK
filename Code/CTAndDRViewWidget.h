#pragma once

#include <QWidget>
#include "ui_CTAndDRViewWidget.h"
#include "BaseView.h"
class CTAndDRViewWidget : public BaseView
{
    Q_OBJECT

public:
    CTAndDRViewWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~CTAndDRViewWidget();
    virtual void AttachView();

private:
    Ui::CTAndDRViewWidget ui;
};
