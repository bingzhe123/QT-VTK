/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : ��½����
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
	***   Description:     IPS��¼�ɹ�
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LoginSuccess();
	void ChangeBackGroundImage();
	void ChangeHideImage();
	void LoginTimeOut();
	/*-----------------------------------------------------------
	***   Description:     �л��û��¼�
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ChangeUser();
public slots:

	/*-----------------------------------------------------------
	***   Description:     ��¼��ť����
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void Login();
private:
	void keyPressEvent(QKeyEvent*event);
	time_t time;
    Ui::IPSLoginClass ui;
	/*-----------------------------------------------------------
	***   Description:     ��¼�������ô�С
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void ReGeometry(QWidget*widget);
	/*-----------------------------------------------------------
	***   Description:     ��¼���ݴ���
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
	***   Description:     �����¼���
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LoginProcess(int value);
	/*-----------------------------------------------------------
	***   Description:     ��������Ͽ�����
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void LostConnect();
};
