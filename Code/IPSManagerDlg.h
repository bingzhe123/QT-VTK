/*-----------------------------------------------------------
***   Description:
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
#include <QDialog>
#include "ui_IPSManagerDlg.h"
#include "ModuleContainer.h"
#include "IPSManagerCenterWidget.h"
#include <qlabel.h>
#include <qbuttongroup.h>
#include <vector>
using std::vector;

class IPSManagerDlg : public QDialog 
{
    Q_OBJECT

public:
    IPSManagerDlg(std::shared_ptr< Container> spContainer, QWidget *parent = Q_NULLPTR);
    ~IPSManagerDlg();
    void paintEvent(QPaintEvent *event);
    //**************************************************
    //	brief   :÷ÿ‘ÿresizeEvent…Ë÷√radionbuttonŒª÷√ 
    //	in      :QResizeEvent£∫envet
    //	out     :
    //	return  : 
    //**************************************************
    virtual void resizeEvent(QResizeEvent *event) override;


    virtual void AttachView();

	void closeEvent(QEvent * event);

	void ClearImage();

private:
    Ui::IPSManagerDlg ui;
    std::shared_ptr<Container> m_spContainer;
    std::shared_ptr<IPSManagerCenterWidget> m_spCenterWidget;
    std::shared_ptr<QLabel> m_spIcon;
    std::shared_ptr<QLabel> m_spName;
    std::shared_ptr<QPushButton> m_spBtnReturn;

};
