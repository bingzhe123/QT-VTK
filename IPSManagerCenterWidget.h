/*-----------------------------------------------------------
***   Description:
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
#ifndef  IPSMANAGERCENTERWIDGET_H_
#define IPSMANAGERCENTERWIDGET_H_

#include <QWidget>
#include <QDateTimeEdit>
#include <QScrollBar>
#include <QDir>
#include <QMessageBox>
#include "ui_IPSManagerCenterWidget.h"

#include "BaseView.h"
#include "commondef.h"
#include "IPSManagerImageWidget.h"
#include "vtkRenderwindow.h"
#include "Cell3D.h"
#include "Cell2D.h"
#include "IPSToolsBar.h"
#include "ManageCheckViewWidget.h"
#include "DRCell.h"
#include "SliceCell.h"
#include "ManageStatus.h"
#include "ResultViewWidgets.h"
#include "IPSThreadReciver.h"
#include "IPSDownloadDicom.h"

class IPSManagerCenterWidget : public BaseView
{
    Q_OBJECT

public:
    IPSManagerCenterWidget(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~IPSManagerCenterWidget();
    //**************************************************
    //	brief   :重载resizeEvent
    //	in      :QResizeEvent：envet
    //	out     :
    //	return  : 
    //**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;

    /*-----------------------------------------------------------
    ***   Description:初始化界面
    ***   OUTPUT     :
    ***   INPUT      :
    ------------------------------------------------------------*/
    virtual void AttachView();
	/*-----------------------------------------------------------
	***   Description:界面关闭时处理
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void Close();
	/*-----------------------------------------------------------
	***   Description:下载并显示图像
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void DownloadShow(QString);
	bool isDirExist(QString fullPath);
	void OnLoadDicomImages(QString session_id);
	void OnClearDicomImages();
private slots:
    void OnHorizontalSlideChange(int iPos);
    void OnVerticalSliderChange(int iPos);
private:
    Ui::IPSManagerCenterWidget ui;
	void IPSManagerCenterWidget::showEvent(QEvent*event);
signals:
	void DisableButton();
	void ClearOprator();
	void ClearItems();
};
#endif // ! IPSMANAGERCENTERWIDGET.h
