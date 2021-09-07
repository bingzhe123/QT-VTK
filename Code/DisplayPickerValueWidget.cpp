#include "DisplayPickerValueWidget.h"

DisplayPickerValueWidget::DisplayPickerValueWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    QPalette myPalette;
    myPalette.setBrush(backgroundRole(), QColor(10, 25, 69));
    this->setPalette(myPalette);
    ui.widget->setObjectName("wpicker");
    ui.widget->setStyleSheet("#wpicker{ background:rgba(10,10,10,0.8);border-radius:20px;}");
    ui.widget->setFixedWidth(250);
    ui.widget->setFixedHeight(40);
}

DisplayPickerValueWidget::~DisplayPickerValueWidget()
{
}
///*-----------------------------------------------------------
//***   Description:���µ�ǰ��ʾ��ֵ��ͨ��pixcelֵ����������ֵ 
//***   OUTPUT     :iValue,��Ҫ���µ�pixcelֵ
//***   INPUT      :
//------------------------------------------------------------*/
//void DisplayPickerValueWidget::UpdatePixelValue(short iValue)
//{     
//    ui.label_z->setText(QString::number(iValue));
//    ui.label_p->setText(QString::number(iValue));
//}
//**************************************************
//	brief   :����ͳ����Ϣ 
//	in      :��ǰѡ��ĵ���Ϣ
//	out     :
//	return  : 
//**************************************************
void DisplayPickerValueWidget::UpdateStatisticInfo(const vector<short> &&vecPixel)
{
    ui.label_z->setText(QString::number(vecPixel[0]));
    ui.label_p->setText(QString::number(vecPixel[0]));
}