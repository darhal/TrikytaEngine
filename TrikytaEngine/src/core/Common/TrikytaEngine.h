#pragma once
#include <core/Common/EngineInstance.h>
#include <memory>
#include <string>

class TrikytaEngine
{
public:
	static std::shared_ptr<class Game> Create(const ENGINE_CONFIG&);
	//static std::shared_ptr<class Game> Create(ENGINE_CONFIG&);
	static class EngineInstance* getEngine() { return G_Engine; };
private:
	static void init(class Game* _Engine);
	static class EngineInstance* G_Engine;
};

