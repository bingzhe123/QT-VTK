#include "AutoCheckingWidget.h"
#include "IPSAPP.h"

AutoCheckingWidget::AutoCheckingWidget(QWidget *parent)
    : QWidget(parent),BaseView(nullptr)
{
    m_pApp = (IPSAPP*)parent;
    ui.setupUi(this);
    connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(Test()));
    connect(this, SIGNAL(ChangeView(View)), parent, SLOT(OnChangeView(View)), Qt::QueuedConnection);


}

AutoCheckingWidget::~AutoCheckingWidget()
{

}
void AutoCheckingWidget::Test()
{
    emit ChangeView(View::MainView);
}