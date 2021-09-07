/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 08. APR 2021
   Description     : ÉÏ²¿×´Ì¬À¸
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/

#ifndef  TOPSTATEBARWIDGET_H_
#define  TOPSTATEBARWIDGET_H_

#include <QWidget>
#include "ui_TopStateBar.h"
#include "BaseView.h"
#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QDateTime>
#include <QFontDataBase>
#include <qpainter.h>
#include <QDebug>


class TopStateBarWidget :public BaseView
{
    Q_OBJECT

public:
    TopStateBarWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~TopStateBarWidget();
    void paintEvent(QPaintEvent *event);
	QLabel *checkmod;
	void ChangeAIShow(bool p);
	void AI();
	void Manual();

	void High();

	void Low();

	void Edge();

	void Color();

	void Black();

	void Organic();

	void IC();
	
public slots:
    void OnCTView();
    void OnDRView();
    void OnMainView();
	void ContrastColor();
	void UpdateScaling(float fzoom);
	
private:
    Ui::TopStateBar ui;
	bool contrast;
};





#endif // TOPSTATEBARWIDGET_H_ !   

