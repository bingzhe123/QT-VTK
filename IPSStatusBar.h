/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : 判图操作倒计时等
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#pragma once

#include <QtWidgets/QWidget>
#include <QStyle>
#include <QTimer>
#include <QDateTime>
#include <QFontDataBase>
#include <qpainter.h>
#include <QDebug>
#include <QPushButton>

#include "ui_IPSStatusBar.h"
#include "BaseView.h"
#include "IPSManagerDlg.h"

class IPSStatusBar : public BaseView
{
    Q_OBJECT

public:
    IPSStatusBar(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
	enum modelIndex
	{
		ct,
		dr,
		ct_dr
	};
	enum manageIndex
	{
		ips,
		scs
	};
	enum tipStatus
	{
		tipOn,
		tipOff
	};

    //**************************************************
    //	brief   :重绘背景色，否则子窗口的背景色不被应用上去 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void paintEvent(QPaintEvent *event);
	void showmanagement();
	QPushButton*change_user_button,*deviceInfo;
	void ChangeCurrentPackagkseID(QString);
	IPSManagerDlg*m_pIPSManager = nullptr;
	~IPSStatusBar();
private:
	Ui::IPSStatusBar ui;
	void OnCtButtonClicked();
	void OnDrButtonClicked();
	void OnDrCtButtonClicked();

	void OnIpsManageButtonClicked();
	modelIndex modelindex;
	void UpdateButtonStyleSheet();
	void showtime();
	tipStatus tipstatus;
	void setTipStatus(tipStatus);
   
public slots:
	void changeTipStatus(tipStatus);
	void OnCTView();
	void OnDRView();
	void OnMainView();
	void SetUserID(QString id);
	void ChangeCheckStatus(bool);
	void OnRecallButtonClicked();
	void OnResetButtonClicked();
	

signals:
	void modelchanged(modelIndex);
	void managechanged(manageIndex);	
	void VertiDisable();
	void StopRecall();
	void Recall();

};
