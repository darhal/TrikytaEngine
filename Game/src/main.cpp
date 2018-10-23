#include <core/Common/TrikytaEngine.h>
#include <cstdio>
//#include <windows.h>
#include <LStateManager/LStateManager.h>
#include <core/Common/defines.h>

#ifndef __DEBUG__

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	auto EngineInstance = TrikytaEngine::Create(1024, 768);
	return 0;
}

#else


int main()
{
	//auto GameInstance = TrikytaEngine::Create(1024, 768);
	using namespace LuaEngine;
	ENGINE_CONFIG config = LStateManager::GetLStateManager()->LLoadConfig(LStateManager::GetLuaState());

	auto GameInstance = TrikytaEngine::Create(config);

	return 0;
};

#endif*/