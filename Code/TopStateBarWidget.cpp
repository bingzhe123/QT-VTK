#include "TopStateBarWidget.h"
/*-----------------------------------------------------------
***   Description:     构造顶部状态栏
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
TopStateBarWidget::TopStateBarWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
	contrast = false;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"TopStateBarWidget::TopStateBarWidget");
	checkmod = ui.label_2;
    ui.setupUi(this);
    setFixedHeight(60);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("QWidget{background:#0;}");
    connect(this, SIGNAL(ChangeView(View)), parent, SLOT(OnChangeView(View)));

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TopStateBarWidget::TopStateBarWidget");
}     

/*-----------------------------------------------------------
***   Description:     析构顶部状态栏
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
TopStateBarWidget::~TopStateBarWidget()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TopStateBarWidget::~TopStateBarWidget");
}

/*-----------------------------------------------------------
***   Description:     重绘事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TopStateBarWidget::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
    //painter.fillRect(rect(), QColor(68, 68, 68);
	
    auto r = rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient linearGradient(r.left(), r.top(), r.right(), r.bottom());
    linearGradient.setColorAt(0, "#854ECA");
    //linearGradient.setColorAt(0.6, Qt::green);
    linearGradient.setColorAt(1.0, "#2080FF");
    painter.setBrush(QBrush(linearGradient));
    painter.fillRect(r, QBrush(linearGradient));

}

void TopStateBarWidget::ChangeAIShow(bool p)
{
	if (p)
		AI();
	else
		Manual();
}

void TopStateBarWidget::AI()
{
	ui.label_2->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/ai.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(),"Switch AI Check Mod");
}

void TopStateBarWidget::Manual()
{
	ui.label_2->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/manual.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(),"Switch Manual Check Mod");
}
 
void TopStateBarWidget::High()
{
	ui.label_enhence->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/high.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch HighProcess Mod");
}
void TopStateBarWidget::Low()
{
	ui.label_enhence->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/low.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch LowProcess Mod");
}
void TopStateBarWidget::Edge()
{
	ui.label_enhence->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/enhenceedge.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch EdgeEnhence Mod");
}
void TopStateBarWidget::Color()
{
	ui.label_process->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/color.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch Colorful Mod");
}
void TopStateBarWidget::Black()
{
	ui.label_process->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/black.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch Gray Mod");
}
void TopStateBarWidget::Organic()
{
	ui.label_process->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/rmor.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch Anti-Organic Mod");
}
void TopStateBarWidget::IC()
{
	ui.label_process->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/rmim.png);border-radius:4px;");
	LOG_INFO(GESP::DebugLogger::GetInstance(), "Switch Anti-IC Mod");
}
/*-----------------------------------------------------------
***   Description:     切换至CT界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TopStateBarWidget::OnCTView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TopStateBarWidget::OnCTView");
    emit ChangeView(View::CTVIEW);
}

/*-----------------------------------------------------------
***   Description:     切换至DR界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TopStateBarWidget::OnDRView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TopStateBarWidget::OnDRView");
    emit ChangeView(View::DRVIEW);
}

/*-----------------------------------------------------------
***   Description:     切换至主界面
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void TopStateBarWidget::OnMainView()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "TopStateBarWidget::OnMainView");
    emit ChangeView(View::MainView);
}
void TopStateBarWidget::ContrastColor()
{
	if (contrast)
	{
		ui.label_neg->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/neg.png);border-radius:4px");
	}
	else 
	{
		ui.label_neg->setStyleSheet("background:#FFFFFF;border-image:url(:/img/res/topstatus/normal.png);border-radius:4px");
	}
}

void TopStateBarWidget::UpdateScaling(float fScale)
{
	auto sText = QString::number(fScale, 'f', 2);
	ui.Scaling->setText(sText);
}
 