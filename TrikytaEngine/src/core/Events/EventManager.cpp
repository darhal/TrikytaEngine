#include "EventManager.h"
#include "core/Common/EngineInstance.h"

EventManager* EventManager::_EventManager = 0;

EventManager* EventManager::GetEventManager()
{
	if (_EventManager == nullptr)
	{
		_EventManager = new EventManager();
	}
	return _EventManager;
}

void EventManager::HandleSDLEvents(SDL_Event& p_Event, EngineInstance* p_Engine)
{
	while (SDL_PollEvent(&p_Event) != 0) {
		if (p_Event.type == SDL_QUIT) { p_Engine->setEngineState(false); p_Engine->On_Engine_Quit(); };
		if (p_Event.type == SDL_KEYDOWN || p_Event.type == SDL_KEYUP) {
			for (auto inputCallbackFunc : *(m_InputCallbacks)) {
				inputCallbackFunc(p_Event.key.keysym.sym, p_Event.type);
			}
		}
		if (p_Event.type == SDL_MOUSEBUTTONDOWN || p_Event.type == SDL_MOUSEBUTTONUP || p_Event.type == SDL_MOUSEWHEEL) {
			for (auto mouseClickCallbackFunc : *(m_MouseClickCallbacks)) {
				Vec2i ClickMousePosition(p_Event.button.x, p_Event.button.y);
				mouseClickCallbackFunc(p_Event.type, p_Event.button.button, ClickMousePosition);
			}
		}
		if (p_Event.type == SDL_MOUSEMOTION) {
			for (auto mouseMoveCallbackFunc : *(m_MouseMoveCallbacks)) {
				Vec2i LastMousePosition(p_Event.motion.x, p_Event.motion.y);
				mouseMoveCallbackFunc(LastMousePosition);
			}
		}
	}
}
