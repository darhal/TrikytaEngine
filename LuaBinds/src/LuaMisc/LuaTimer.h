#pragma once

struct lua_State;

namespace LuaEngine {
	class LuaTimer
	{
	public:
		static void LoadTimerSystem();
		static LuaTimer* GetTimerManager();
		static void LoadTimerFunction();
		static int CreateTimer(lua_State *L);
		static void RemoveFuncFromRegistery(int);
		void CallTimerFunc(int);
	private:
		static LuaTimer* _LUA_TIMER_;
		
	};
}