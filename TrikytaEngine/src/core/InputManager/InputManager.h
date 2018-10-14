#pragma once

class InputManager
{
public:
	static InputManager* initInputManager();
	static InputManager* getInputManager();

	void ActivateInput(bool);
private:
	InputManager():activeCount(0){}
	static InputManager* _InputManager;
	int activeCount;
};