#pragma once

#include <QtWidgets/QDialog>
#include "ui_IPSToolsBarTips.h"

class IPSToolsBarTips : public QDialog
{
    Q_OBJECT

public:
    IPSToolsBarTips(QWidget *parent = Q_NULLPTR);
	void showtext(int x, int y, const char*a);
private:
    Ui::IPSToolsBarTipsClass ui;
};
