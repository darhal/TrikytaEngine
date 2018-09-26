#pragma once

struct lua_State;

namespace LuaEngine {
	class LuaConsole
	{
	public:
		static void LoadConsoleSystem();
		static LuaConsole* GetConsoleManager();
		static void LoadConsoleFunction();

		static int outputConsole(lua_State* L);
		static int outputConsoleWithColors(lua_State* L);
	private:
		static LuaConsole* _LUA_CONSOLE_;
	};
}