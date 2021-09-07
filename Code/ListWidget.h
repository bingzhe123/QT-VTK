#pragma once
#include <QPainter>
#include <QStyle>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QScreen>
#include <QWidget>
#include <QApplication>
#include <QLayout>

#include "gespips.h"
class ListWidget
{
public:
	ListWidget::ListWidget(QListWidget*list_widget = nullptr, int code = 0, double z = 0.1, double ¦Ñ = 0.1, double m = 1, QString type = QString::fromLocal8Bit("µ¶¾ß"));
	~ListWidget();
	QWidget *widget;
	QListWidgetItem *item;
	QPushButton *reject_button;
	QLabel *identifier_label, *icon_label, *weight_label, *property_¦Ñ_label,*property_z_label,* type_label;
};
