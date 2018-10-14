#include "UIManager.h"
#include "UIBase.h"
#include "Events/UIEventHandler.h"

using namespace UI;

Manager* Manager::_Manager = nullptr;
EventHandler* Manager::_EventManager = nullptr;

Manager* Manager::InitManager()
{
	_EventManager = EventHandler::InitEventManager();
	_Manager = new Manager();
	return _Manager;
}

Manager* Manager::getManager()
{
	return _Manager;
}

EventHandler* Manager::getEventManager()
{
	return _EventManager;
 }

Manager::ObjectsVec& Manager::getUIContainer()
{ 
	return getManager()->m_UIContainer;
}

void Manager::addElement(Base* p_UiObj)
{
	Manager::getUIContainer().push_back(p_UiObj);
}

void Manager::removeElement(Base*)
{

}