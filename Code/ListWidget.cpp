#include "ListWidget.h"

ListWidget::ListWidget(QListWidget*list_widget,int code,double z,double ρ,double m,QString type)
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"Item add");

	QScreen *screen = QGuiApplication::primaryScreen();
	QRect mm = screen->availableGeometry();

	widget = new QWidget(list_widget);

	widget->setStyleSheet("background:transparent;");

	identifier_label = new QLabel(widget);
	identifier_label->setText(QString::fromLocal8Bit("可疑物") + QString::number(code));
	identifier_label->setStyleSheet("QLabel{color:red;font:16px PingFang SC;}");

	icon_label = new QLabel(widget);
	icon_label->setStyleSheet("QLabel{background:url(:/img/res/resultview/warning.png) center no-repeat 21px 19px ;}");
	icon_label->setFixedSize(24, 24);

	property_z_label = new QLabel(widget);
	property_z_label->setText(QString::fromLocal8Bit(" z:") + QString::number(z));
	property_z_label->setStyleSheet("QLabel{font:14px PingFang SC;}");

	property_ρ_label = new QLabel(widget);
	property_ρ_label->setText(QString::fromLocal8Bit(" ρ:") + QString::number(ρ));
	property_ρ_label->setStyleSheet("QLabel{font:14px PingFang SC;}");

	weight_label = new QLabel(widget);
	weight_label->setText(QString::fromLocal8Bit(" 重量:") + QString::number(m)+QString::fromLocal8Bit("kg"));
	weight_label->setStyleSheet("QLabel{font:14px PingFang SC;}");

	type_label = new QLabel(widget);
	type_label->setText(QString::fromLocal8Bit(" 类别:") + type);
	type_label->setStyleSheet("QLabel{font:14px PingFang SC;}");

	reject_button = new QPushButton(widget);
	reject_button->setStyleSheet("QPushButton{border-radius:2px;background:#642AB5 url(:/img/res/resultview/button.png) center no-repeat;}"
	"QPushButton::hover{border-radius:2px;background:#9254DE url(:/img/res/resultview/button.png) center no-repeat;}"
	"QPushButton::pressed{border-radius:2px;background:#391085 url(:/img/res/resultview/button.png) center no-repeat;}");
	reject_button->setProperty("code", code);
	reject_button->setFixedSize(35,24);

	QFont font;
	font.setPixelSize(1);
	reject_button->setFont(font);
	QHBoxLayout *horizontal_layout = new QHBoxLayout;
	horizontal_layout->setContentsMargins(20, 2, 10, 2);

	horizontal_layout->addWidget(icon_label);
	horizontal_layout->addWidget(identifier_label);
	horizontal_layout->addStretch();
	horizontal_layout->addWidget(property_z_label);
	horizontal_layout->addStretch();
	horizontal_layout->addWidget(property_ρ_label);
	horizontal_layout->addStretch();
	horizontal_layout->addWidget(weight_label);
	horizontal_layout->addStretch();
	horizontal_layout->addWidget(type_label);
	horizontal_layout->addStretch();
	horizontal_layout->addWidget(reject_button);
	horizontal_layout->addStretch();

	horizontal_layout->setSpacing(0);
	horizontal_layout->setMargin(5);
	reject_button->setFixedWidth(reject_button->height() / 2 * 3);
	icon_label->setFixedWidth(icon_label->height());
	widget->setLayout(horizontal_layout);
	property_ρ_label->setFixedWidth(list_widget->geometry().width() / 8);
	property_z_label->setFixedWidth(list_widget->geometry().width() / 8);
	type_label->setFixedWidth(list_widget->geometry().width() / 6);
	weight_label->setFixedWidth(list_widget->geometry().width() / 5);
}

ListWidget::~ListWidget()
{
	delete widget;
	delete item;
	delete reject_button;
}
