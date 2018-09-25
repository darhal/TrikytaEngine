#include <map>
#include <chrono>
#include <SDL/SDL_image.h>
#include "EngineInstance.h"
#include <core/Objects/Object.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Events/EventManager.h>
#include <core/Objects/ObjectHandler.h>
#include <LStateManager/LStateManager.h>
#include "core/Utility/TimerManager.h"

bool EngineInstance::Init()
{
	On_Engine_Pre_Init();// CALL PREINIT
	EventManager::GetEventManager()->HandleOnEnginePreInitEvents();

	Log("Trikyta Engine initializing. Please wait...");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Log("Unable to Init SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		Log("Unable to Init hinting: %s", SDL_GetError());
		return false;
	}

	if ((m_Window = SDL_CreateWindow(
		m_Wnd_Name.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN)
		) == NULL) {
		Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	m_PrimarySurface = SDL_GetWindowSurface(m_Window);

	if ((m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		Log("Unable to create renderer");
		return false;
	}

	// Initialize image loading for PNGs
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		Log("Unable to init SDL_image: %s", IMG_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0xFF);
	m_EngineState = true;
	const Physics2D::PhysicsEngineParams a = {{ 0, 10.f }, 1 / 60.f, 8, 3 };
	this->InitPhysics(a);

	Log("Engine is ready...");

	LuaEngine::LStateManager::GetLStateManager()->LoadScripts();
	TimerManager::InitTimerManager(); // Init timer system!
	EventManager::GetEventManager()->HandleOnEngineLoadEvents(); // Handle this events on the manager
	On_Engine_Init(); // CALL INIT

	Log("Engine is active and rendering!");
	return true;
}

void EngineInstance::InitPhysics(const Physics2D::PhysicsEngineParams& p_Params) 
{
	Physics2D::PhysicsEngine::createPhysicsWorld(p_Params);
	Log("Physics Engine is ready!...");
}

void EngineInstance::EngineLogic() 
{
	SDL_Event Event;
	LastTick = std::chrono::system_clock::now();
	while (m_EngineState) {
		EventManager::GetEventManager()->HandleSDLEvents(Event, this);
		Render();
		SDL_Delay(1);
	}
}

void EngineInstance::Render() 
{
	std::chrono::time_point<std::chrono::system_clock> TimeNow = std::chrono::system_clock::now();
	std::chrono::duration<float> dt = (TimeNow - LastTick);
	float dtf = dt.count(); //delta time float

	SDL_RenderClear(m_Renderer);

	for (auto itr : *(ObjectHandler::GetObjectHandler()))
	{
		itr->render(dtf);
	}

	On_Engine_Render(dtf);
	Physics2D::PhysicsEngine::GetPhysicsWorld()->update(dtf);

	SDL_RenderPresent(m_Renderer);
	TimerManager::Update();
	LastTick = std::chrono::system_clock::now();
	EventManager::GetEventManager()->HandleOnEngineRenderEvents(dtf);
}



/*EngineInstance EngineInstance::g_Engine; <- put this on top
EngineInstance* EngineInstance::GetEngineInstance()
{
	return &EngineInstance::g_Engine;;
}*/