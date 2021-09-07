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
//	brief   :改变锁定状态 
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void ChangeLockStatus(int);
//**************************************************
//	brief   :锁定解锁图像 
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void LockUnlock();
	
//**************************************************
//	brief   :改变sessionID
//	in      : 
//	out     :
//	return  : 
//**************************************************
	void ChangeID(QString a);
	
	//**************************************************
	//	brief   :删除图像
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void DeleteImage();
	//**************************************************
	//	brief   :创建打印内容
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	const QPixmap CreateDoc();
	//**************************************************
	//	brief   :打印格式
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
	//	brief   : 导出图像到文件夹 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void exportImage();
signals:
	void DeleteOne();
};

//**************************************************
//	brief   : 拷贝源文件夹内容到目的文件夹 
//	in      : 
//	out     :
//	return  : 
//**************************************************
bool copyDirectory(const QString& srcPath, const QString& dstPath, bool coverFileIfExist);
