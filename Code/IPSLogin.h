/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : 登陆界面
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtSvg>
#include <QtWidgets>
#include <QPushButton>
#include "qwidget.h"
#include "IPSAPP.h"
#include "ui_IPSLogin.h"
#include "gespips.h"
#include "IPSThreadReciver.h"


string getTime(string a);
class IPSLogin : public QMainWindow
{
    Q_OBJECT

public:
	IPSAPP*mainwindow;
	IPSLogin(IPSAPP *w,QWidget *parent = Q_NULLPTR );
	~IPSLogin();
	/*-----------------------------------------------------------
	***   Description:     IPS登录成功
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LoginSuccess();
	void ChangeBackGroundImage();
	void ChangeHideImage();
	void LoginTimeOut();
	/*-----------------------------------------------------------
	***   Description:     切换用户事件
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ChangeUser();
public slots:

	/*-----------------------------------------------------------
	***   Description:     登录按钮按下
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void Login();
private:
	void keyPressEvent(QKeyEvent*event);
	time_t time;
    Ui::IPSLoginClass ui;
	/*-----------------------------------------------------------
	***   Description:     登录界面重置大小
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ReGeometry(QWidget*widget);
	/*-----------------------------------------------------------
	***   Description:     登录数据处理
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LoginDataProcess();
	void paintEvent(QEvent*event);
	QPixmap *backgroundimage[3];
	QString login_user_id;
	QTimer *hidetime;
	QTimer *loginOvertime;
	QTimer *imagetime;
	QRect screensize;
	QScreen *screen;
	double ratiox, ratioy;
	int alph;
	int imageindex;
	bool login;
public slots:
	/*-----------------------------------------------------------
	***   Description:     处理登录结果
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LoginProcess(int value);
	/*-----------------------------------------------------------
	***   Description:     与服务器断开连接
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LostConnect();
};
