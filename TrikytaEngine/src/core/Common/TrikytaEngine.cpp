#include "TrikytaEngine.h"
#include "../../Game/src/Game.h" //TODO: Need to be changed!
#include <memory>

EngineInstance* TrikytaEngine::G_Engine;

std::shared_ptr<Game> TrikytaEngine::Create(std::string p_Wnd_Name, const int p_WindowWidth, const int p_WindowHeight)
{
	auto _Engine = std::make_shared<Game>(p_Wnd_Name, p_WindowWidth, p_WindowHeight);
	TrikytaEngine::init(_Engine.get());
	return 0;
};

std::shared_ptr<Game> TrikytaEngine::Create(const int p_WindowWidth, const int p_WindowHeight)
{
	auto _Engine = std::make_shared<Game>(DEFAULT_WND_NAME, p_WindowWidth, p_WindowHeight);
	TrikytaEngine::init(_Engine.get());
	return 0;
};

void TrikytaEngine::init(Game* _Engine)
{
	Log("Setting G_Engine");
	TrikytaEngine::G_Engine = _Engine;
	if (_Engine->Init()) {
		_Engine->EngineLogic();
	}
	else {
		Log("Error Initing the engine...");
		exit(1);
	}

}

