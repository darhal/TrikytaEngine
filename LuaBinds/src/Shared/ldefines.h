#pragma once
#include <Core/Common/defines.h>

#define SCRIPT_META_PATH    "scripts/meta.txt"
#define SCRIPTS_PATH		"scripts/"

#ifdef __DEBUG__
#define LogL(TYPE, ...) printf("[TrikytaEngine-%s][LUA ENGINE][%s]: ", Utility::getDateNow(), TYPE); printf(__VA_ARGS__); printf("\n");
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

