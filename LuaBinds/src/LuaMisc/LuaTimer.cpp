#include <LStateManager/LStateManager.h>
#include <core/Utility/TimerManager.h>
#include "Shared/ldefines.h"
#include "LuaTimer.h"
#include <lua.hpp>

using namespace LuaEngine;

LuaTimer* LuaTimer::_LUA_TIMER_ = nullptr;

void LuaTimer::LoadTimerSystem() 
{
	_LUA_TIMER_ = new LuaTimer();

	LoadTimerFunction();
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
	luaL_checktype(L, 1, LUA_TFUNCTION);
	int callPeriod = (int)lua_tonumber(L, 2);
	int numCalls = (int)lua_tonumber(L, 3);
	lua_pop(L, 2);
	int timerFuncIndex = luaL_ref(L, LUA_REGISTRYINDEX);
	auto t = TimerManager::CreateTimer(timerFuncIndex, callPeriod, numCalls, true);
	return 0;
}

void LuaTimer::CallTimerFunc(int p_FuncIndex)
{
	auto L = LStateManager::GetLuaState();
	LogL("INFO", "Index : %d ", p_FuncIndex);
	lua_rawgeti(L, LUA_REGISTRYINDEX, p_FuncIndex);
	if (lua_pcall(L, 0, 0, 0))
	{
		LogL("ERROR", "Attempt running function : %s", lua_tostring(L, -1));
	}
}