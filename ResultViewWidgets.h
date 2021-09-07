/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 19. APR 2021
   Description     : 判图结果
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/

#ifndef  RESULTVIEWWIDGETS_H_
#define  RESULTVIEWWIDGETS_H_


#include <QMutex>
#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QDateTime>
#include <QFontDataBase>
#include <QWidget>
#include <QLabel>
#include <QScrollBar>
#include <QListWidget>
#include "qlistwidget.h"
#include <QPushButton>
#include <QScreen>
#include "ListWidget.h"
#include "ui_ResultViewWidgets.h"
#include "BaseView.h"
#include "SuspiciousObject.h"

class ResultViewWidgets : public BaseView
{
    Q_OBJECT

public:
    
	ResultViewWidgets(ContainerPtr spContainer, QWidget * parent = Q_NULLPTR, bool mode = true);
	~ResultViewWidgets();
	void CountChanged();
	QListWidget *list_widget;
	QLabel* total_number;
	QLabel* current_selected;
	void ChangeSuspiciousShow();
	void AddItemToSuspiciousContainer(double x, double y, double z, double l, double w, double h,QString uid);
	void SetAICount();
	void ChangeAIShow(bool show);
	void ShowReview(QString sessionID,int lock);
	void ClearSuspicious();
	void SetReviewID(QString);
Q_SIGNALS:
	void SelectSuspiciousObject(QString uid);
	void RemoveSuspiciousObject(QString uid);
signals:
	void ChangeLockStatus(int);
	/*-----------------------------------------------------------
	***   Description:判图结束清除界面中的图像
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ClearIamge();
private:
    Ui::ResultViewWidgets ui;
	void AddItemToWidgetList(QListWidget*, int code = 0, double z = 0.1, double ρ = 0.1, double m = 1, QString type = QString::fromLocal8Bit("刀具"));
	void RemoveItem(int code);

	void resizeEvent(QResizeEvent * );
	void ResetHeight();
	int counts;
	QLabel *frame;
	QString currentReviewID;
	int susp_number;
	bool reviewMode;
};

#endif // RESULTVIEWWIDGETS_H_ !   

