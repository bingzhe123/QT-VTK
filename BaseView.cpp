#include "BaseView.h"
#include "IPSAPP.h"



/*-----------------------------------------------------------
***   Description:     �������湹��
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
BaseView::BaseView(std::shared_ptr< Container> spContainer, QWidget *parent)
    :QWidget(parent)
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "BaseView::BaseView");

    //ע��ö�����ͣ��Ա����qt���źŲۻ���
    qRegisterMetaType<View>("View");
    m_spContainer = spContainer;
    m_pApp = (IPSAPP*)parent;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "BaseView::BaseView");
}

/*-----------------------------------------------------------
***   Description:     ��������ճ��
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void BaseView::AttachView( )
{
}
