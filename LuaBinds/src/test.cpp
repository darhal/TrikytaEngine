#include <lua.hpp>
#include <memory>
#include "test.h"

WndConfig loadLuaConfigs()
{
	system("cd");
	int status;
	lua_State *L = luaL_newstate();   /* opens Lua */
	luaL_openlibs(L);
	status = luaL_loadfile(L, "scripts/config.lua");
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
	printf("H:%d // W:%d // name:%s", wnd_h, wnd_w, wnd_name);
	//lua_pushcfunction(L, CreateSprite);
	//lua_setglobal(L, "CreateSprite");
	return WndConfig{ wnd_w, wnd_h, wnd_name };
}

void CreateSprite(lua_State *L)
{

}