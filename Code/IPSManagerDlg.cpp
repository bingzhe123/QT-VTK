#include "IPSManagerDlg.h"
#include "TopStateBarWidget.h"
#include "DRCell.h"
#include "SliceCell.h"
#include "Cell3D.h"
#include "IPSToolsBar.h"
#include "CheckResultWidget.h"

IPSManagerDlg::IPSManagerDlg(std::shared_ptr< Container> spContainer, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_spContainer = spContainer;
   
    //setFixedSize(1024, 768);

    Qt::WindowFlags flags = windowFlags();

    setWindowFlags(flags | Qt::WindowFullscreenButtonHint | Qt::FramelessWindowHint | Qt::Dialog );
    raise();
    m_spCenterWidget = std::make_shared<IPSManagerCenterWidget>(spContainer, this);

    QPalette pal(m_spCenterWidget->palette());

    //设置背景
    pal.setColor(QPalette::Background, Qt::white);
    m_spCenterWidget->setAutoFillBackground(true);
    m_spCenterWidget->setPalette(pal);
    m_spCenterWidget->show();

    m_spIcon = std::make_shared<QLabel>(this);
    m_spIcon->setStyleSheet("border-image:url(:/img/res/mainwindow/LOGO.png);");
    m_spIcon->show();
    m_spName = std::make_shared<QLabel>(this);
    m_spName->setStyleSheet("color:rgb(255, 255, 255);\ntext-align: left;font:24px;");
    m_spName->setText(QStringLiteral("IPS管理"));
    m_spName->show();

    m_spBtnReturn = std::make_shared<QPushButton>(this);
    m_spBtnReturn->setStyleSheet("QPushButton{border-image:url(:/img/res/mainwindow/return.png);border-radius:4px;background:#FFFFFF;}");
	

    m_spBtnReturn->show();
	m_spBtnReturn->setFixedWidth(m_spBtnReturn->height());
	connect(m_spBtnReturn.get(), &QPushButton::clicked, this, &IPSManagerDlg::close);
	connect(m_spBtnReturn.get(), &QPushButton::clicked, m_spCenterWidget.get(), &IPSManagerCenterWidget::Close);
}

IPSManagerDlg::~IPSManagerDlg()
{
}

/*-----------------------------------------------------------
***   Description:     重绘事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSManagerDlg::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
    //painter.fillRect(rect(), QColor(68, 68, 68);

    LOG_DEBUG(GESP::DebugLogger::GetInstance(),"TopStateBarWidget::paintEvent");
    auto r = rect();
    r.setHeight(320);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient linearGradient(r.left(), r.top(), r.right(), r.bottom());
    linearGradient.setColorAt(0, "#854ECA");
    //linearGradient.setColorAt(0.6, Qt::green);
    linearGradient.setColorAt(1.0, "#2080FF");
    painter.setBrush(QBrush(linearGradient));
    painter.fillRect(r, QBrush(linearGradient));
    LOG_DEBUG(GESP::DebugLogger::GetInstance(),"TopStateBarWidget::paintEvent");
   
}

//**************************************************
//	brief   :重载resizeEvent 
//	in      :QResizeEvent：envet
//	out     :
//	return  : 
//**************************************************
void IPSManagerDlg::resizeEvent(QResizeEvent *event)
{
    auto rect = this->geometry();
    int iOffsetx = 20;
    int iOffsety = 10;
    m_spIcon->move(iOffsetx, iOffsety);
    m_spName->move(iOffsetx + 120, iOffsety);
    m_spBtnReturn->move(iOffsetx + 250, iOffsety);

    int y = 50;              
    int w = rect.width() - iOffsetx * 2;
    int h = rect.height() - y - iOffsetx;
    m_spCenterWidget->setFixedSize(w, h);
    m_spCenterWidget->move(iOffsetx, y);
}

/*-----------------------------------------------------------
***   Description:    粘贴窗口
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSManagerDlg::AttachView()
{
    LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSManagerDlg::AttachView");
    m_spCenterWidget->AttachView();
}

void IPSManagerDlg::closeEvent(QEvent * event)
{
	m_spCenterWidget->Close();

}

void IPSManagerDlg::ClearImage()
{
	m_spCenterWidget->OnClearDicomImages();
}