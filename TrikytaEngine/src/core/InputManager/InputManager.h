#pragma once
#include "core/Common/Vec2.h"
#include <SDL/SDL.h>

class InputManager
{
public:
	static InputManager* initInputManager();
	static InputManager* getInputManager();

	void ActivateInput(bool);
	void DrawCursor(float);
	void setCurosrPosition(const Vec2i&, const Vec2i&);
private:
	InputManager():
		activeCount(0), LastDeltaTime(0.f), IsShowCursor(true), mCursorDim(SDL_Rect{0,0,0,0})
	{}
	static InputManager* _InputManager;
	int activeCount;
	float LastDeltaTime;
	bool IsShowCursor;
	SDL_Rect mCursorDim;
};