#include <map>
#include <chrono>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "EngineInstance.h"
#include <core/Objects/Object.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Events/EventManager.h>
#include <core/Objects/ObjectHandler.h>
#include <LStateManager/LStateManager.h>
#include "core/Utility/TimerManager.h"
#include "misc/Console/Console.h"
#include "core/Common/defines.h"
#include "UI/UIManager.h"
#include "core/InputManager/InputManager.h"

//#include <GL/glew.h>
/*
bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}*/

bool EngineInstance::Init()
{
	On_Engine_Pre_Init();// CALL PREINIT
	//EventManager::GetEventManager()->HandleOnEnginePreInitEvents();

	LogTerminal("Trikyta Engine initializing. Please wait...");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LogTerminal("Unable to Init SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		LogTerminal("Unable to Init hinting: %s", SDL_GetError());
		return false;
	}
	
	if ((m_Window = SDL_CreateWindow(
		m_EngineConfig.WINDOW_NAME,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_EngineConfig.WINDOW_WIDTH, m_EngineConfig.WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)
		) == NULL) {
		LogTerminal("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}
	
	// Create an OpenGL context associated with the window.
	//SDL_GLContext glcontext = SDL_GL_CreateContext(m_Window);
	//SetOpenGLAttributes();

	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	//SDL_GL_DeleteContext(glcontext);

	m_PrimarySurface = SDL_GetWindowSurface(m_Window);

	if ((m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		LogTerminal("Unable to create renderer");
		return false;
	}

	// Initialize image loading for PNGs
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		LogTerminal("Unable to init SDL_image: %s", IMG_GetError());
		return false;
	}
	TTF_Init();
	SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0xFF);
	LogTerminal("__________________________________________________________________________");
	UI::Manager::InitManager();
	Console::InitConsole();
	InputManager::initInputManager()->ActivateInput(false); // init and desactivate!
	LogInfoConsole("Engine is ready...");
	
	const Physics2D::PhysicsEngineParams phyParams = {{ m_EngineConfig.PHYSICS_WORLD_GRAVITY_X, m_EngineConfig.PHYSICS_WORLD_GRAVITY_Y }, m_EngineConfig.PHYSICS_TIME_STEP,
		m_EngineConfig.PHYSICS_VELOCITY_ITERATIONS, m_EngineConfig.PHYSICS_POSITION_ITERATIONS,  m_EngineConfig.PHYSICS_DEBUG};
	this->InitPhysics(phyParams);
	TimerManager::InitTimerManager(); // Init timer system!
	
	LuaEngine::LStateManager::GetLStateManager()->LoadScripts();
	EventManager::GetEventManager()->HandleOnEngineLoadEvents(); // Handle this events on the manager
	On_Engine_Init(); // CALL INIT

	LogInfoConsole("Engine is active and rendering!");
	m_EngineState = true;
	
	auto rect = SDL_Rect{0,0,(int)m_EngineConfig.WINDOW_WIDTH,(int)m_EngineConfig.WINDOW_HEIGHT};
	SDL_RenderSetViewport(m_Renderer, &rect);
	return true;
}

void EngineInstance::InitPhysics(const Physics2D::PhysicsEngineParams& p_Params) 
{
	Physics2D::PhysicsEngine::createPhysicsWorld(p_Params);
	LogInfoConsole("Physics Engine is ready!...");
}

void EngineInstance::EngineLogic() 
{
	SDL_Event Event;
	LastTick = std::chrono::system_clock::now();
	while (m_EngineState) {
		EventManager::GetEventManager()->HandleSDLEvents(Event, this);
		Render();
		TimerManager::Update();
		SDL_Delay(m_EngineConfig.ACCELERATION_RATE); // 1ms
	}
}

void EngineInstance::Render() 
{
	std::chrono::time_point<std::chrono::system_clock> TimeNow = std::chrono::system_clock::now();
	std::chrono::duration<float> dt = (TimeNow - LastTick);
	float dtf = dt.count();
	SDL_RenderClear(m_Renderer);
	for (auto& itr : *(ObjectHandler::GetObjectHandler()))
	{
		itr->render(dtf);
	}
	On_Engine_Render(dtf);
	SDL_GL_SwapWindow(m_Window);
	Physics2D::PhysicsEngine::GetPhysicsWorld()->update(dtf);
	Console::getConsole()->Draw(dtf);
	SDL_RenderPresent(m_Renderer);
	LastTick = std::chrono::system_clock::now();
	EventManager::GetEventManager()->HandleOnEngineRenderEvents(dtf);
}



/*EngineInstance EngineInstance::g_Engine; <- put this on top
EngineInstance* EngineInstance::GetEngineInstance()
{
	return &EngineInstance::g_Engine;;
}*/