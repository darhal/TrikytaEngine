#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <lua.hpp>
#include "LuaBridge/LuaBridge.h"
#include "LStateManager.h"
#include "Shared/ldefines.h"
#include "LuaMisc/LuaTimer.h"
#include "LuaPhysics/LuaBody.h"
#include "LuaEvents/LuaEvents.h"
#include "LuaDrawable/LuaSprite.h"
#include "LuaDrawable/LuaAnimation.h"
#include "LuaMisc/LuaConsole.h"
#include "LuaUI/LuaText.h"
#include "LuaCore/ErrorManager.h"
#include <core/Common/defines.h>

using namespace LuaEngine;



/*using namespace luabridge;
void printMessage(const std::string& s) {
	std::cout << s << std::endl;
}

int main() {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L).addFunction("printMessage", printMessage);
	luaL_dofile(L, "script.lua");
	lua_pcall(L, 0, 0, 0);
	LuaRef sumNumbers = getGlobal(L, "sumNumbers");
	int result = sumNumbers(5, 4);
	std::cout << "Result:" << result << std::endl;
	system("pause");
}*/

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

ENGINE_CONFIG LStateManager::LLoadConfig(lua_State* L)
{
	LogL("INFO", "Loading configuration file...");
	int status = luaL_loadfile(L, "scripts/config.lua");
	if (status || lua_pcall(L, 0, 0, 0)) {
		LogTerminal("[WARNING]:	Couldn't load file: %s\n", lua_tostring(L, -1));
		LogTerminal("LOADING DEFAULT SETTINGS!");
		return ENGINE_CONFIG{};
	}

	lua_getglobal(L, "WINDOW_WIDTH");
	lua_getglobal(L, "WINDOW_HEIGHT");
	lua_getglobal(L, "WINDOW_NAME");
	lua_getglobal(L, "ACCELERATION_RATE");
	lua_getglobal(L, "PHYSICS_DEBUG");
	lua_getglobal(L, "PHYSICS_TIME_STEP");
	lua_getglobal(L, "PHYSICS_VELOCITY_ITERATIONS");
	lua_getglobal(L, "PHYSICS_POSITION_ITERATIONS");
	lua_getglobal(L, "PHYSICS_WORLD_GRAVITY_X");
	lua_getglobal(L, "PHYSICS_WORLD_GRAVITY_Y");

	unsigned int WINDOW_WIDTH		= (unsigned int)lua_tonumber(L, 1);
	unsigned int WINDOW_HEIGHT		= (unsigned int)lua_tonumber(L, 2);
	const char* WINDOW_NAME			= lua_tostring(L, 3);
	unsigned int ACCELERATION_RATE	= (unsigned int)lua_tonumber(L, 4);
	bool PHYSICS_DEBUG				= (bool)lua_toboolean(L, 5);
	float PHYSICS_TIME_STEP			= (float)lua_tonumber(L, 6);
	int PHYSICS_VELOCITY_ITERATIONS = (int)lua_tonumber(L, 7);
	int PHYSICS_POSITION_ITERATIONS = (int)lua_tonumber(L, 8);
	float PHYSICS_WORLD_GRAVITY_X	= (float)lua_tonumber(L, 9);
	float PHYSICS_WORLD_GRAVITY_Y	= (float)lua_tonumber(L, 10);

	LogTerminal("______________________ CONFIGURATIONS LOADED _____________________________");
	LogTerminal("WINDOW_WIDTH:................. %d", WINDOW_WIDTH);
	LogTerminal("WINDOW_HEIGHT:................ %d", WINDOW_HEIGHT);
	LogTerminal("WINDOW_NAME:.................. %s", WINDOW_NAME);
	LogTerminal("ACCELERATION_RATE:............ %d", ACCELERATION_RATE);
	LogTerminal("PHYSICS_DEBUG:................ %s", PHYSICS_DEBUG ? "Enabled" : "Disabled");
	LogTerminal("PHYSICS_TIME_STEP:............ %f", PHYSICS_TIME_STEP);
	LogTerminal("PHYSICS_VELOCITY_ITERATIONS:.. %d", PHYSICS_VELOCITY_ITERATIONS);
	LogTerminal("PHYSICS_POSITION_ITERATIONS:.. %d", PHYSICS_POSITION_ITERATIONS);
	LogTerminal("PHYSICS_WORLD_GRAVITY_X:...... %f", PHYSICS_WORLD_GRAVITY_X);
	LogTerminal("PHYSICS_WORLD_GRAVITY_Y:...... %f", PHYSICS_WORLD_GRAVITY_Y);
	LogTerminal("__________________________________________________________________________");
	
	return ENGINE_CONFIG
	{ 
		WINDOW_WIDTH , WINDOW_HEIGHT, 
		WINDOW_NAME , ACCELERATION_RATE, 
		PHYSICS_DEBUG , PHYSICS_TIME_STEP, 
		PHYSICS_VELOCITY_ITERATIONS , PHYSICS_POSITION_ITERATIONS, 
		PHYSICS_WORLD_GRAVITY_X, PHYSICS_WORLD_GRAVITY_Y 
	};
}

std::shared_ptr<std::vector<std::string>> LStateManager::LoadScriptList()
{
	//luaA_open(_LUA_STATE_);
	LStateManager::LoadingTrikytaEnv(); // LOAD GLOBAL FUNCTIONS

	using namespace std;
	ifstream ScriptFile;
	ScriptFile.open(SCRIPT_META_PATH);
	string line;
	std::shared_ptr<std::vector<std::string>> ScriptsToLoad = std::make_shared<std::vector<std::string>>();
	ScriptsToLoad.get()->reserve(1);
	LogConsole(MESSAGE_TYPE::INFO, "Parsing scripts in meta...");
	if (ScriptFile.is_open())
	{
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
		LogConsole(MESSAGE_TYPE::ERROR, "Couldn't load the meta file !");
		LogConsole(MESSAGE_TYPE::ERROR, "Expected meta file at %s", SCRIPT_META_PATH);
	}
	return ScriptsToLoad;
}

void LStateManager::LoadScripts()
{
	std::shared_ptr<std::vector<std::string>> ScriptsToLoad = LStateManager::LoadScriptList();
	for (auto _ScrtipToLoad : *ScriptsToLoad.get()) {

		int status = luaL_loadfile(_LUA_STATE_, (SCRIPTS_PATH +_ScrtipToLoad).c_str());
		LogConsole(MESSAGE_TYPE::INFO, "Compiling script : %s", _ScrtipToLoad.c_str())
		if (status || lua_pcall(_LUA_STATE_, 0, 0, 0)) {
			LogConsole(MESSAGE_TYPE::ERROR, "Couldn't load file: %s", lua_tostring(_LUA_STATE_, -1));
			return;
		}
	}
	
}

void LStateManager::RestertScript(std::string p_ScriptName)
{
	int status = luaL_loadfile(_LUA_STATE_, (SCRIPTS_PATH + p_ScriptName).c_str());
	LogConsole(MESSAGE_TYPE::INFO, "Compiling script : %s", p_ScriptName.c_str())
	if (status || lua_pcall(_LUA_STATE_, 0, 0, 0)) {
		LogConsole(MESSAGE_TYPE::ERROR, "Couldn't load file: %s", lua_tostring(_LUA_STATE_, -1));
		return;
	}
	LogConsole(MESSAGE_TYPE::INFO, "%s restarted successfully", p_ScriptName.c_str())
}

void LStateManager::LoadingTrikytaEnv()
{
	LogConsole(MESSAGE_TYPE::INFO, "Loading Trikyta Enviroument");

	ErrorManager::InitErrorManager();
	LuaEvents::GetLuaEventMnager()->RegisterLuaEventManager();
	LuaSprite::LoadSpriteSystem();
	LuaAnimation::LoadAnimationFunctions();
	LuaTimer::LoadTimerSystem();
	LuaBody::LoadPhysicsBodySystem();
	LuaConsole::LoadConsoleSystem();
	LuaText::LoadTextSystem();
}



