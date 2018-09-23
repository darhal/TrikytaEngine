#include "Shared/ldefines.h"
#include "LStateManager.h"
#include "LuaEvents/LuaEvents.h"
#include "LuaDrawable/LuaSprite.h"
#include "LuaDrawable/LuaAnimation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iostream>
#include <lua.hpp>

using namespace LuaEngine;

LStateManager* LStateManager::_Lua_Engine = nullptr;

LStateManager* LStateManager::GetLStateManager()
{
	if (_Lua_Engine == nullptr) 
	{
		_Lua_Engine = new LStateManager();
	}
	return _Lua_Engine;
}

lua_State* LStateManager::GetLuaState()
{
	return LStateManager::GetLStateManager()->_LUA_STATE_;
}

LStateManager::LStateManager()
{
	_LUA_STATE_ = luaL_newstate();
	luaL_openlibs(_LUA_STATE_);
}

WND_CONFIG LStateManager::LLoadConfig(lua_State* L)
{
	LogL("INFO", "Loading configuration file...");
	int status = luaL_loadfile(L, "scripts/config.lua");
	if (status || lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		return WND_CONFIG{ 0, 0, "FAIL" };
	}
	lua_getglobal(L, "WINDOW_WIDTH");
	lua_getglobal(L, "WINDOW_HEIGHT");
	lua_getglobal(L, "WINDOW_NAME");
	const char* wnd_name = lua_tostring(L, -1);
	int wnd_h = (int)lua_tonumber(L, -2);
	int wnd_w = (int)lua_tonumber(L, -3);
	return WND_CONFIG{ wnd_w, wnd_h, wnd_name };
}

std::shared_ptr<std::vector<std::string>> LStateManager::LoadScriptList()
{
	LStateManager::LoadingTrikytaEnv(); // LOAD GLOBAL FUNCTIONS

	using namespace std;
	ifstream ScriptFile;
	ScriptFile.open(SCRIPT_META_PATH);
	string line;
	std::shared_ptr<std::vector<std::string>> ScriptsToLoad = std::make_shared<std::vector<std::string>>();
	ScriptsToLoad.get()->reserve(1);
	LogL("INFO", "Parsing scripts in meta...");
	if (ScriptFile.is_open())
	{
		//Log("[INFO] Reading file %s ...", ScriptFile.c_str());
		while (getline(ScriptFile, line))
		{
			string ScriptPath;
			int i = 0;
			bool isFirstLine = true;
			istringstream LineStream(line);
			while (getline(LineStream, ScriptPath, ';'))
			{
				auto const pos = LineStream.str().find_last_of(';');
				auto _ScrtipToLoad = LineStream.str().substr(0, pos);
				ScriptsToLoad.get()->emplace_back(_ScrtipToLoad);
			}
		}
	}else {
		LogL("ERROR", "Couldn't load the meta file !");
		LogL("ERROR", "Expected meta file at %s", SCRIPT_META_PATH);
	}
	return ScriptsToLoad;
}

void LStateManager::LoadScripts()
{
	std::shared_ptr<std::vector<std::string>> ScriptsToLoad = LStateManager::LoadScriptList();
	for (auto _ScrtipToLoad : *ScriptsToLoad.get()) {

		int status = luaL_loadfile(_LUA_STATE_, (SCRIPTS_PATH +_ScrtipToLoad).c_str());
		LogL("INFO", "Compiling script : %s", _ScrtipToLoad.c_str())
		if (status || lua_pcall(_LUA_STATE_, 0, 0, 0)) {
			LogL("ERROR","Couldn't load file: %s\n", lua_tostring(_LUA_STATE_, -1));
			return;
		}
	}
}

void LStateManager::LoadingTrikytaEnv()
{
	LogL("INFO", "Loading Trikyta Enviroument");

	LuaEvents::GetLuaEventMnager()->RegisterLuaEventManager();
	LuaSprite::LoadSpriteSystem();
	LuaAnimation::LoadAnimationFunctions();
}



