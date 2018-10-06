#include "LStateManager/LStateManager.h"
#include "LuaMisc/LuaConsole.h"
#include <misc/Console/Console.h>
#include <UI/UIText.h>
#include <algorithm>
#include <lua.hpp>

using namespace LuaEngine;

LuaConsole* LuaConsole::_LUA_CONSOLE_ = nullptr;

void LuaConsole::LoadConsoleSystem()
{
	if (_LUA_CONSOLE_ == nullptr) {
		_LUA_CONSOLE_ = new LuaConsole();
	}
	LuaConsole::LoadConsoleFunction();
}

LuaConsole* LuaConsole::GetConsoleManager()
{
	return _LUA_CONSOLE_;
}

void LuaConsole::LoadConsoleFunction()
{
	auto L = LStateManager::GetLuaState();

	lua_pushcfunction(L, LuaConsole::outputConsole);
	lua_setglobal(L, "outputConsole");

	lua_pushcfunction(L, LuaConsole::outputConsoleWithColors);
	lua_setglobal(L, "outputCConsole");
}

int LuaConsole::outputConsole(lua_State* L)
{
	std::string msgType = std::string(lua_tostring(L, 2));
	std::transform(msgType.begin(), msgType.end(), msgType.begin(), ::tolower);
	MESSAGE_TYPE m_Type;
	if (msgType == "error") {
		m_Type = MESSAGE_TYPE::ERROR;
	}else if (msgType == "info") {
		m_Type = MESSAGE_TYPE::INFO;
	}else if (msgType == "warning") {
		m_Type = MESSAGE_TYPE::WARNING;
	}
	Console::getConsole()->outputConsole(lua_tostring(L, 1), m_Type);
	return 0;
}

int LuaConsole::outputConsoleWithColors(lua_State* L)
{
	Console::getConsole()->outputConsole(lua_tostring(L, 1), Color{ (uint8)lua_tonumber(L, 2), (uint8)lua_tonumber(L, 3) , (uint8)lua_tonumber(L, 4),  255});
	return 0;
}