#include "BaseView.h"
#include "IPSAPP.h"



/*-----------------------------------------------------------
***   Description:     基础界面构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
BaseView::BaseView(std::shared_ptr< Container> spContainer, QWidget *parent)
    :QWidget(parent)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "BaseView::BaseView");

    //注册枚举类型，以便符合qt的信号槽机制
    qRegisterMetaType<View>("View");
    m_spContainer = spContainer;
    m_pApp = (IPSAPP*)parent;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "BaseView::BaseView");
}

/*-----------------------------------------------------------
***   Description:     基础界面粘贴
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void BaseView::AttachView( )
{
}
