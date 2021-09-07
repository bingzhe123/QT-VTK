#pragma once

#include <QtWidgets/QWidget>
#include <QDateTimeEdit>
#include <QCombobox>
#include <QListWidget>
#include <QLabel>
#include <QScrollBar>
#include <QPushButton>
#include <QMutex>
#include <queue>

#include "gespips.h"
#include "commondef.h"
#include "ui_ManageStatus.h"

class ManageStatus : public QWidget
{
    Q_OBJECT

public:
    ManageStatus(QWidget *parent = Q_NULLPTR);
	void InsertReviewList();
	void InsertReviewUser();
	void Clear();
	void ClearOprator();
	void ClearItems();
	QListWidget* list_area;
private:
    Ui::ManageStatusClass ui;
	QDateTimeEdit* begin_time, *end_time;
	QComboBox* operator_box;

	QLabel *start_icon, *stop_icon, *user_icon, *pac_icon, *start_time, *stop_time, *user_select, *pac_info;
	void GetReviewList();

	void GetSessionID();
	
signals:
	void SelectPacage(QString);
	void ShowSessionID(QString);

};
