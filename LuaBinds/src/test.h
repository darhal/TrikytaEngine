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
	void OnInput(int key, unsigned int state);
	static int MoveSprite(lua_State* L);
	static int GetSpritePos(lua_State* L);
}

/*class LuaBindsManager
{
	LuaBindsManager() 
	{
		
	}
};*/