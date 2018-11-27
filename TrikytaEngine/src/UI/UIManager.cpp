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

Manager::ObjectsVec& Manager::getUIRenderableContainer()
{
	return getManager()->m_UIRenderableContainer;
}

void Manager::addElement(Base* p_UiObj, bool isRenderable)
{
	if (isRenderable) {
		Manager::getUIRenderableContainer().push_back(p_UiObj);
	} else {
		Manager::getUIContainer().push_back(p_UiObj);
	}
}

void Manager::renderElements(float dt)
{
	for (auto& ui_elem : Manager::getUIRenderableContainer()) {
		ui_elem->render(dt);
	}
}

void Manager::removeElement(Base* p_UiObj, bool isRenderable)
{
	if (isRenderable) {
		Manager::getUIRenderableContainer().remove(p_UiObj);
	}else {
		Manager::getUIContainer().remove(p_UiObj);
	}
}