#include "IPSManagerImageWidget.h"
#include <qpushbutton.h>

IPSManagerImageWidget::IPSManagerImageWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
    : BaseView(spContainer, parent)
{
    ui.setupUi(this);
    ui.listWidget_package->setFixedWidth(150);
    QListWidgetItem *pButtonItem = new QListWidgetItem();
    pButtonItem->setSelected(false);
    auto flags = pButtonItem->flags();
    pButtonItem->setFlags(flags & ~Qt::ItemIsEnabled &~Qt::ItemIsUserCheckable);
    auto pButton = new QPushButton(this);
    pButton->setText(QStringLiteral("全选"));    
    pButton->setFixedWidth(100);
    ui.listWidget_package->addItem(pButtonItem);
    ui.listWidget_package->setItemWidget(pButtonItem, pButton);
}

IPSManagerImageWidget::~IPSManagerImageWidget()
{
}
