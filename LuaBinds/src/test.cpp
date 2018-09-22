#include <memory>
#include <core/Drawable/Sprite.h>
#include "test.h"

using namespace LuaBinds;
lua_State* L;

static int LuaBinds::CreateSprite(lua_State *L)
{
	const char* path = lua_tostring(L, -5);  /* get argument */
	int w = (int)lua_tonumber(L, -4); 
	int h = (int)lua_tonumber(L, -3);
	int x = (int)lua_tonumber(L, -2);
	int y = (int)lua_tonumber(L, -1);
	//printf("\n\n%d, %d, %d %d\n\n", x, y, w, h);
	auto obj = Sprite::Create(path, Vec2i(w, h), Vec2i(x, y));
	return 0;
}

void LuaBinds::Lua_CallOnEngineLoad()
{
	lua_getglobal(L, "OnEngineLoad");  /* function to be called */
	if (lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "ERROR: %s\n", lua_tostring(L, -1));
	}
	lua_settop(L, 0);
	printf("\n\nLUA STACK SIZE : %d \n\n", lua_gettop(L));
}

WndConfig LuaBinds::loadLuaConfigs(lua_State* L)
{
	
	int status = luaL_loadfile(L, "scripts/config.lua");
	if (status || lua_pcall(L, 0, 0, 0)) {
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		return WndConfig{0,0,"FAIL"};
	}
	lua_getglobal(L, "WINDOW_WIDTH");
	lua_getglobal(L, "WINDOW_HEIGHT");
	lua_getglobal(L, "WINDOW_NAME");
	const char* wnd_name = lua_tostring(L, -1);
	int wnd_h = (int)lua_tonumber(L, -2);
	int wnd_w = (int)lua_tonumber(L, -3);
	lua_pushcfunction(L, LuaBinds::CreateSprite);
	lua_setglobal(L, "CreateSprite");
	lua_settop(L, 0);
	printf("\n\nLUA STACK SIZE : %d \n\n", lua_gettop(L));
	return WndConfig{ wnd_w, wnd_h, wnd_name };
}

WndConfig LuaBinds::LoadLua()
{
	L = luaL_newstate();   /* opens Lua */
	luaL_openlibs(L);
	return loadLuaConfigs(L);
}