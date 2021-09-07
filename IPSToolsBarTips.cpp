#include "IPSToolsBarTips.h"


IPSToolsBarTips::IPSToolsBarTips(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	ui.label->setStyleSheet("QLabel{ backgroud:#FFFFFF;color:#000000; };");
}

void IPSToolsBarTips::showtext(int x, int y, const char * a)
{
	if (isHidden())
	{
		ui.label->setText(a);
		setGeometry(x, y, 40, 25);
		show();
	}
}
