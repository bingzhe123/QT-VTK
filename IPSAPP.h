/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 15. APR 2021
   Description     :
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#ifndef  IPSAPP_H_
#define  IPSAPP_H_

#include <QtSvg>
#include <QtWidgets/QMainWindow>
#include "ui_IPSAPP.h"
#include "MainViewWidget.h"
#include "commondef.h"
#include "ModuleContainer.h"
#include <share.h>
#include "gespips.h"
#include "IPSStatusBar.h"
#include "IPSToolsBar.h"
#include <QTimer>
#include <qcursor.h>
#include "IPSToolsBarTips.h"
#include "AutoCheckingModule.h"
#include "Cell2D.h"
#include "Cell3D.h"    
#include "DRCell.h"
#include "DRViewWidget.h"
#include "TopStateBarWidget.h"

#include "CTViewWidgets.h"
#include "ResultViewWidgets.h"
#include "CTAndDRViewWidget.h"
#include <qpainter.h>
#include <qdir.h>
#include "TCPThread.h"
#include "JsonHelper.h"
#include "qstyleoption.h"

#include "CheckResultWidget.h"
#include "DataModule.h"
#include "SliceCell.h"
#include "DisplayPickerValueWidget.h"

#include "IPSDownloadDicom.h"
#include "IPSDeviceInformation.h"

class CommandContext;

class IPSAPP : public QMainWindow
{
    Q_OBJECT

public:
    IPSAPP(QWidget *parent = Q_NULLPTR);
    std::shared_ptr< Container> GetContainer();


	void GetHoriSignal();
	void GetVertiSignal();

    virtual void paintEvent(QPaintEvent *event);

    //**************************************************
    //	brief   :接收通讯线程发送的消息 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void customEvent(QEvent *event);

    //**************************************************
    //	brief   :退出APP时候需要停止vtk消息循环 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void closeEvent(QCloseEvent* event);

	//**************************************************
	//	brief   : DR图片下载 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadDRImage(QString);

	//**************************************************
	//	brief   : 开始判图
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void StartManualCheck(QString sessionid);
	void StartSecondCheck(QString sessionid);
	//**************************************************
	//	brief   : 判图结束
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void CheckStop();
	//**************************************************
	//	brief   : CT图像下载 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadCTImage(QString);

	//**************************************************
	//	brief   :  照片下载
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadPhoto(QString);
	//**************************************************
	//	brief   : 其它信息下载 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadOtherInfo(QString);
	//**************************************************
	//	brief   : 图像回溯上一张 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void ChangeRecallImage();

	void StopRecallImage();
	
	~IPSAPP();
	
private slots:
    //**************************************************
    //	brief   :  视图切换函数
    //	in      :  view：需要切换的view
    //	out     :  N.A
    //	return  :  N.A
    //**************************************************
    void OnChangeView(View view);

	//**************************************************
	//	brief   :  载入下载的图像
	//	in      :  
	//	out     :  
	//	return  :  
	//**************************************************
	void OnLoadDicomImages(QString session_id);
   virtual void moveEvent(QMoveEvent * event);
   
public:
    //**************************************************
    //	brief   : 切换到主界面
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2MainView();

    //**************************************************
    //	brief   : 切换CTview
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2CTView();

    //**************************************************
    //	brief   :切换到DRView 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2DRView();

	void Change2ManageView();
    //**************************************************
    //	brief   :处理BE发送的消息 
    //	in      :context:消息的内容，json格式的字符串
    //	out     :
    //	return  : 
    //**************************************************
    void BeMsgCommandHandler(const CommandContext &context);

	//**************************************************
	//	brief   : 归类ct，dr图像，放入不同的文件夹 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	bool DividCTAndDR(const string folder,const string ctFolder,const string drFolder);

	//**************************************************
	//	brief   : 清空文件夹里的所有文件 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ClearAllFiles(QString path);

	//**************************************************
	//	brief   : 判断文件夹是否存在，不存在则创建 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	bool isDirExist(QString fullPath);

	//**************************************************
	//	brief   : 替换掉模拟器里生成的CT图
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void InsteadSimuCT(const string& src,const string& dest);

private:
    //数据module容器
    std::shared_ptr< Container> m_spContainer;
	bool DR_downloaded;
	bool CT_downloaded;
    string m_currentView = "";

    QPixmap *m_pBkPixmap1 = nullptr;
	IPSStatusBar*statusbar;

	QTimer *tipstimer;

	DataModule* m_spDataModule;

	QCursor *cursor;
	MainViewWidget*MainWidget;
	CheckResultWidget*checkarea;
	CTViewWidgets*CTWidgets;

public:
    Ui::IPSAPPClass ui;
	//int x, y;
signals:
	void ChangeUserSignal();
	void StartCheck(QString);
	void ChangeEquipIDSignal(QString);
	void ChangeCheckStatus(bool);
	void StopReview();
	void EnableToolsbar();
	void DisableToolsbar();
	void StopRecallCheck();
};

#endif // IPSAPP_H_ !   

