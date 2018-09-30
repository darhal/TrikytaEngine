#include "TrikytaEngine.h"
#include "../../Game/src/Game.h" //TODO: Need to be changed!
#include <memory>

EngineInstance* TrikytaEngine::G_Engine;

std::shared_ptr<Game> TrikytaEngine::Create(ENGINE_CONFIG& p_Config)
{
	auto _Engine = std::make_shared<Game>(p_Config);
	TrikytaEngine::init(_Engine.get());
	return 0;
};

/*std::shared_ptr<Game> TrikytaEngine::Create(ENGINE_CONFIG& p_Config)
{
	auto _Engine = std::make_shared<Game>(p_Config);
	TrikytaEngine::init(_Engine.get());
	return 0;
};*/

void TrikytaEngine::init(Game* _Engine)
{
	LogTerminal("Setting G_Engine");
	TrikytaEngine::G_Engine = _Engine;
	if (_Engine->Init()) {
		_Engine->EngineLogic();
	}
	else {
		LogTerminal("Error Initing the engine...");
		exit(1);
	}

}

