#include "LuaTimer.h"
#include <lua.hpp>
#include <LStateManager/LStateManager.h>

using namespace LuaEngine;

void LuaTimer::LoadTimerSystem() 
{
	_LUA_TIMER_ = new LuaTimer();
}

LuaTimer* LuaTimer::GetTimerManager()
{
	return _LUA_TIMER_;
}

void LuaTimer::LoadTimerFunction() 
{
	auto L = LStateManager::GetLuaState();

	lua_pushcfunction(L, LuaTimer::CreateTimer);
	lua_setglobal(L, "setTimer");
}

int LuaTimer::CreateTimer(lua_State *L) 
{
	luaL_checktype(L, -1, LUA_TFUNCTION);
	int timerFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	int callPeriod = lua_tonumber(L, 1);
	int numCalls = lua_tonumber(L, 2);

	return 0;
}