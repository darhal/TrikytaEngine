#include "UIEventHandler.h"
#include "UI/UIManager.h"
#include "UI/UIBase.h"

using namespace UI;

EventHandler* EventHandler::InitEventManager()
{
	return new EventHandler();
}

void EventHandler::BlockEvents(bool b)
{
	m_BlockProcess = b;
}

void EventHandler::processEvents(SDL_Event& e)
{
	for (auto obj : Manager::getUIContainer()) {
		if (obj != nullptr)
			obj->PorcessEvents(e);
	}
}