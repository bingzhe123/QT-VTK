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
    //	brief   :����ͨѶ�̷߳��͵���Ϣ 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void customEvent(QEvent *event);

    //**************************************************
    //	brief   :�˳�APPʱ����Ҫֹͣvtk��Ϣѭ�� 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    virtual void closeEvent(QCloseEvent* event);

	//**************************************************
	//	brief   : DRͼƬ���� 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadDRImage(QString);

	//**************************************************
	//	brief   : ��ʼ��ͼ
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void StartManualCheck(QString sessionid);
	void StartSecondCheck(QString sessionid);
	//**************************************************
	//	brief   : ��ͼ����
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void CheckStop();
	//**************************************************
	//	brief   : CTͼ������ 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadCTImage(QString);

	//**************************************************
	//	brief   :  ��Ƭ����
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadPhoto(QString);
	//**************************************************
	//	brief   : ������Ϣ���� 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void DownloadOtherInfo(QString);
	//**************************************************
	//	brief   : ͼ�������һ�� 
	//	in      :
	//	out     :
	//	return  : 
	//**************************************************
	void ChangeRecallImage();

	void StopRecallImage();
	
	~IPSAPP();
	
private slots:
    //**************************************************
    //	brief   :  ��ͼ�л�����
    //	in      :  view����Ҫ�л���view
    //	out     :  N.A
    //	return  :  N.A
    //**************************************************
    void OnChangeView(View view);

	//**************************************************
	//	brief   :  �������ص�ͼ��
	//	in      :  
	//	out     :  
	//	return  :  
	//**************************************************
	void OnLoadDicomImages(QString session_id);
   virtual void moveEvent(QMoveEvent * event);
   
public:
    //**************************************************
    //	brief   : �л���������
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2MainView();

    //**************************************************
    //	brief   : �л�CTview
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2CTView();

    //**************************************************
    //	brief   :�л���DRView 
    //	in      :
    //	out     :
    //	return  : 
    //**************************************************
    void Change2DRView();

	void Change2ManageView();
    //**************************************************
    //	brief   :����BE���͵���Ϣ 
    //	in      :context:��Ϣ�����ݣ�json��ʽ���ַ���
    //	out     :
    //	return  : 
    //**************************************************
    void BeMsgCommandHandler(const CommandContext &context);

	//**************************************************
	//	brief   : ����ct��drͼ�񣬷��벻ͬ���ļ��� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	bool DividCTAndDR(const string folder,const string ctFolder,const string drFolder);

	//**************************************************
	//	brief   : ����ļ�����������ļ� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void ClearAllFiles(QString path);

	//**************************************************
	//	brief   : �ж��ļ����Ƿ���ڣ��������򴴽� 
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	bool isDirExist(QString fullPath);

	//**************************************************
	//	brief   : �滻��ģ���������ɵ�CTͼ
	//	in      : 
	//	out     :
	//	return  : 
	//**************************************************
	void InsteadSimuCT(const string& src,const string& dest);

private:
    //����module����
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

