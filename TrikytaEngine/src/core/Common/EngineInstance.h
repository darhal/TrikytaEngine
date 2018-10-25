#pragma once
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <functional> 
#include <SDL/SDL.h>
#include "core/Common/defines.h"

class EngineInstance
{
public:
	/*EngineInstance() :
		m_Wnd_Name(DEFAULT_WND_NAME),
		m_EngineConfig(m_EngineConfig(p_Config),z),
		m_Window(nullptr),
		m_Renderer(nullptr),
		m_PrimarySurface(nullptr)
	{};*/

	EngineInstance(struct ENGINE_CONFIG& p_Config) :
		m_EngineConfig(p_Config),
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

	const unsigned int GetScreenWidth() const { return m_EngineConfig.WINDOW_WIDTH; }
	const unsigned int GetScreenHeight() const { return m_EngineConfig.WINDOW_HEIGHT; }

private:
	struct ENGINE_CONFIG& m_EngineConfig;
	bool m_EngineState;
	std::string m_Wnd_Name;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Surface* m_PrimarySurface;
	std::chrono::time_point<std::chrono::system_clock> LastTick;
};
