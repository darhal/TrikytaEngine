#include <core/Common/TrikytaEngine.h>
#include <cstdio>
#include <LStateManager/LStateManager.h>
#include <core/Common/defines.h>
#ifdef _MSC_VER
#include <windows.h>
#endif

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
	auto GameInstance = TrikytaEngine::Create(LuaEngine::LStateManager::GetLStateManager()->LLoadConfig(LuaEngine::LStateManager::GetLuaState()));
	return 0;
};

#endif