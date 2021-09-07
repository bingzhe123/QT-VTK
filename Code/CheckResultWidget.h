/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : 判图操作倒计时等
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  CHECKRESULTWIDGET_H_
#define  CHECKRESULTWIDGET_H_

#include <ctime>
#include <QWidget>
#include "ui_CheckResultWidget.h"
#include "BaseView.h"
#include <QTimer>
#include <QMessageBox>
#include "IPSThreadReciver.h"
#include <QLabel>
#include <QButtonGroup>

class CheckResultWidget : public BaseView
{
    Q_OBJECT
signals:
	void CheckTimeOut();
	void HoriCheck();
	void VertiCheck();
	void SetAICounts();
	void AutoCheckSwitch(bool);
	void DisableToolsbar();
	void StopCheck();

public:
    CheckResultWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~CheckResultWidget();
	void StartCheck(QString id);
	void ShowTimeLeft(int a);
	void CheckCompleted();
	void CheckCompletedWarning();
	void TimeOutReturn();
	void SetVertiDisable();
	void paintEvent(QPaintEvent*);
	QString eid;
	void SetAutoCheck();
	void SetManualCheck();
	void StopRecallCheck();

public slots:
	void OnHoriCheck();

	
private:
    Ui::CheckResultWidget ui;
	QTimer *time_control;
	int time_left;
	time_t time_start_check;
	QString session_id;
	QLabel* label_time_count;
	QButtonGroup *leftdown, *middown;
};

#endif // CHECKRESULTWIDGET_H_ !   