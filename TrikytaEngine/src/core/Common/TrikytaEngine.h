#pragma once
#include <core/Common/EngineInstance.h>
#include <memory>
#include <string>

class TrikytaEngine
{
public:
	static std::shared_ptr<class Game> Create(std::string p_Wnd_Name, const int p_WindowWidth, const int p_WindowHeight);
	static std::shared_ptr<class Game> Create(const int p_WindowWidth, const int p_WindowHeight);
	static class EngineInstance* getEngine() { return G_Engine; };
private:
	static void init(class Game* _Engine);
	static class EngineInstance* G_Engine;
};

