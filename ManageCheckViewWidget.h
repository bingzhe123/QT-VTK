#pragma once

#include <QtWidgets/QWidget>
#include "ui_ManageCheckViewWidget.h"
#include "BaseView.h"
#include "IPSThreadCoder.h"
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QScreen>
class ManageCheckViewWidget : public BaseView
{
    Q_OBJECT

public:
    ManageCheckViewWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
	QRadioButton* vertiRadio, *horiRadio;
//**************************************************
//	brief   :�ı�����״̬ 
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void ChangeLockStatus(int);
//**************************************************
//	brief   :��������ͼ�� 
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void LockUnlock();
	
//**************************************************
//	brief   :�ı�sessionID
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void ChangeID(QString a);
	
	//**************************************************
	//	brief   :ɾ��ͼ��
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void DeleteImage();
	//**************************************************
	//	brief   :������ӡ����
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	const QPixmap CreateDoc();
	//**************************************************
	//	brief   :��ӡ��ʽ
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void PrintPaint(QPrinter *printer);
	void Print();
private:
	bool lock;
    Ui::ManageCheckViewWidgetClass ui;
	QString sessionID;
    QPushButton* exportButton,*deleteButton,*lockButton,*printButton;
	QPrintPreviewDialog printDialog;
	//**************************************************
	//	brief   : ����ͼ���ļ��� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void exportImage();
signals:
	void DeleteOne();
};

//**************************************************
//	brief   : ����Դ�ļ������ݵ�Ŀ���ļ��� 
//	in      : 
//	out     :
//	return  : 
//**************************************************
bool copyDirectory(const QString& srcPath, const QString& dstPath, bool coverFileIfExist);
