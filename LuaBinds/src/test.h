#pragma once

struct WndConfig 
{
	int w;
	int h;
	const char* name;
};

WndConfig loadLuaConfigs();
void CreateSprite(lua_State *L);