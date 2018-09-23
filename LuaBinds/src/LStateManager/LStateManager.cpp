#include "Shared/ldefines.h"
#include "LStateManager.h"
#include "LuaEvents/LuaEvents.h"
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

	lua_pushcfunction(_LUA_STATE_, LuaBinds::CreateSprite);
	lua_setglobal(_LUA_STATE_, "CreateSprite");

	lua_pushcfunction(_LUA_STATE_, LuaBinds::MoveSprite);
	lua_setglobal(_LUA_STATE_, "SetPos");

	lua_pushcfunction(_LUA_STATE_, LuaBinds::GetSpritePos);
	lua_setglobal(_LUA_STATE_, "GetPos");

	lua_pushcfunction(_LUA_STATE_, LuaBinds::DeletSprite);
	lua_setglobal(_LUA_STATE_, "DelSprite");

}

/***********************************************************************/
#include <core/Drawable/Sprite.h>

static int LuaBinds::CreateSprite(lua_State *L)
{
	const char* path = lua_tostring(L, -5);  /* get argument */
	int w = (int)lua_tonumber(L, -4);
	int h = (int)lua_tonumber(L, -3);
	int x = (int)lua_tonumber(L, -2);
	int y = (int)lua_tonumber(L, -1);
	auto obj = Sprite::Create(path, Vec2i(w, h), Vec2i(x, y));
	lua_pushlightuserdata(L, (void*)obj);
	return 1;
}

void LuaBinds::OnInput(int key, unsigned int state)
{
	auto L = LStateManager::GetLuaState();
	lua_getglobal(L, "OnInput");  /* function to be called */
	lua_pushnumber(L, key);
	lua_pushnumber(L, state);
	if (lua_pcall(L, 2, 0, 0)) {
		fprintf(stderr, "ERROR: %s\n", lua_tostring(L, -1));
	}
	//lua_settop(L, 0);
}

static int LuaBinds::MoveSprite(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	//printf("\n\n x=%d, y=%d, sprt=%p \n\n", x, y,(sprt));
	sprt->setPosition(Vec2i(x, y));
	//lua_settop(L, 0);
	return 0;
}

static int LuaBinds::GetSpritePos(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	Vec2i p = sprt->getPosition();
	lua_pushinteger(L, p.x);
	lua_pushinteger(L, p.y);
	return 2;
}

void LuaBinds::OnRender(float dt)
{
	auto L = LStateManager::GetLuaState();
	lua_getglobal(L, "OnRender");
	lua_pushnumber(L, dt);
	if (lua_pcall(L, 1, 0, 0)) {
		fprintf(stderr, "ERROR: %s\n", lua_tostring(L, -1));
	}
}

static int LuaBinds::DeletSprite(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	printf("\n\n sprt : %p\n\n", sprt);
	if (sprt != NULL) {
		FREE(sprt);
		sprt = NULL;
	}
	return 0;
}


void LuaBinds::Lua_CallOnEngineLoad()
{
	auto L = LStateManager::GetLuaState();
	lua_getglobal(L, "OnEngineLoad");  /* function to be called */
	if (lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "ERROR: %s\n", lua_tostring(L, -1));
	}
	lua_settop(L, 0);
	//printf("\n\nLUA STACK SIZE : %d \n\n", lua_gettop(L));
}


