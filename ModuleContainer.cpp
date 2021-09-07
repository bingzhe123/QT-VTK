#include "ModuleContainer.h"
#include "Cell.h"



/*-----------------------------------------------------------
***   Description:     ��������
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Container::Container()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Container::Container");
}

/*-----------------------------------------------------------
***   Description:     ��������
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Container::~Container()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::~Container");
}

/*-----------------------------------------------------------
***   Description:     ���ģ��
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Container::AddModule(const string & sName, IModule * pModule)
{
    m_mapModule[sName] = pModule;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::AddModule");
}

/*-----------------------------------------------------------
***   Description:     ���widget
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Container::AddWidget(const string & sName, QWidget * pWidget)
{
    m_mapWidget[sName] = pWidget;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::AddWidget");
}

/*-----------------------------------------------------------
***   Description:     ��ȡ���н���
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
std::vector<Cell*> Container::GetAllCell()
{       

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::GetAllCell");
    std::vector<Cell*> vecCells;
    for (auto it : m_mapWidget)
    {
        auto pCell = dynamic_cast<Cell*>(it.second);
        if (pCell != nullptr)
        {
            vecCells.push_back(pCell);
        }
    }
    return vecCells;
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::GetAllCell");
}
