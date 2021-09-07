#include "ModuleContainer.h"
#include "Cell.h"



/*-----------------------------------------------------------
***   Description:     容器构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Container::Container()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "Container::Container");
}

/*-----------------------------------------------------------
***   Description:     容器析构
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
Container::~Container()
{

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::~Container");
}

/*-----------------------------------------------------------
***   Description:     添加模型
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Container::AddModule(const string & sName, IModule * pModule)
{
    m_mapModule[sName] = pModule;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::AddModule");
}

/*-----------------------------------------------------------
***   Description:     添加widget
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void Container::AddWidget(const string & sName, QWidget * pWidget)
{
    m_mapWidget[sName] = pWidget;

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "Container::AddWidget");
}

/*-----------------------------------------------------------
***   Description:     获取所有界面
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
