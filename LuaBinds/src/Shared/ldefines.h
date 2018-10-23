#pragma once
#include <core/Common/defines.h>
#include <algorithm>

#define SCRIPT_META_PATH    "scripts/meta.txt"
#define SCRIPTS_PATH		"scripts/"

#ifdef __DEBUG__
#define LogL(TYPE, ...) printf("[%s][TrikytaEngine][LUA ENGINE][%s]: ", Utility::getDateNow().c_str(), TYPE); printf(__VA_ARGS__); printf("\n");
#else
#define Log(...) ;
#endif



namespace LuaEngine
{
	class LStateManager;
	class LuaEvents;
	class LuaSprite;
	struct WND_CONFIG;
}

