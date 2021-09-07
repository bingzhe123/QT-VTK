#include "RulerWidget.h"
#include "qpainter.h"

RulerWidget::RulerWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
    ui.setupUi(this);
    this->setWindowOpacity(0.3);


    setWindowFlag(Qt::FramelessWindowHint, true);
   /*   QPalette palette;
    QColor color(190, 230, 250);
    color.setAlphaF(0);
    palette.setBrush(this->backgroundRole(), color);
    this->setPalette(palette);
    this->setAutoFillBackground(true);*/
   // setWindowFlags(Qt::FramelessWindowHint);
   /* QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));
    setPalette(pal);*/
}

RulerWidget::~RulerWidget()
{
}
void RulerWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear); 
    p.drawRect(10, 10, 300, 40);
}
