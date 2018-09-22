#pragma once
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <functional> 
#include <SDL/SDL.h>
#include "defines.h"

class EngineInstance
{
public:
	EngineInstance() :
		m_Wnd_Name(DEFAULT_WND_NAME),
		m_WindowWidth(),
		m_WindowHeight(),
		m_Window(nullptr),
		m_Renderer(nullptr),
		m_PrimarySurface(nullptr)
	{};

	EngineInstance(std::string p_Wnd_Name, const int p_WindowWidth, const int p_WindowHeight) :
		m_Wnd_Name(p_Wnd_Name),
		m_WindowWidth(p_WindowWidth),
		m_WindowHeight(p_WindowHeight),
		m_Window(nullptr),
		m_Renderer(nullptr),
		m_PrimarySurface(nullptr)
	{};

	bool Init();
	void Render();
	void EngineLogic();
	void InitPhysics(const Physics2D::PhysicsEngineParams&);

	virtual void On_Engine_Pre_Init() {}
	virtual void On_Engine_Init() {}
	virtual void On_Engine_Render(float) {}
	virtual void On_Engine_Quit() {}

	inline SDL_Window* getWindow() { return m_Window; }
	inline SDL_Renderer* getRenderer() { return m_Renderer; }
	inline SDL_Surface* getPrimarySurface() { return m_PrimarySurface; }
	inline bool getEngineState() { return m_EngineState; }

	inline void setEngineState(bool p_State) { m_EngineState = p_State; }

	const int GetScreenWeight() const { return m_WindowWidth; }
	const int GetScreenHeight() const { return m_WindowHeight; }

private:
	bool m_EngineState;
	std::string m_Wnd_Name;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Surface* m_PrimarySurface;
	std::chrono::time_point<std::chrono::system_clock> LastTick;

	const int m_WindowHeight;
	const int m_WindowWidth;
};
