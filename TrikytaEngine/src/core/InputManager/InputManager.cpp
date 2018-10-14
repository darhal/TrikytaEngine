#include "InputManager.h"
#include <SDL/SDL.h>

InputManager* InputManager::_InputManager = nullptr;

InputManager* InputManager::initInputManager()
{
	_InputManager = new InputManager();
	return _InputManager;
}

InputManager* InputManager::getInputManager()
{
	return _InputManager;
}

void InputManager::ActivateInput(bool isActive)
{
	if (isActive) {
		if (activeCount == 0) {
			SDL_StartTextInput();
		}
		activeCount++;
	}else {
		activeCount--;
	}
	if (activeCount == 0) {
		SDL_StopTextInput();
	}
}
