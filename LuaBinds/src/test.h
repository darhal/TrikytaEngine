#pragma once

#include <lua.hpp>

namespace LuaBinds {


	struct WndConfig
	{
		int w;
		int h;
		const char* name;
	};

	WndConfig loadLuaConfigs(lua_State* L);
	static int CreateSprite(lua_State *L);
	void Lua_CallOnEngineLoad();
	WndConfig LoadLua();
}

/*class LuaBindsManager
{
	LuaBindsManager() 
	{
		
	}
};*/