#pragma once

struct lua_State;

namespace LuaEngine {
	class LuaText
	{
	public:
		static void LoadTextSystem();
		static LuaText* GetTextManager();
		static void LoadTextFunction();

		static int createText(lua_State* L);
		static int setTextColor(lua_State* L);

		static int SetTextPosition(lua_State* L);
		static int SetTextRotationCenter(lua_State* L);
		static int GetTextPosition(lua_State* L);
		static int setText(lua_State* L);
		static int GetTextRotation(lua_State* L);

		static int DeleteText(lua_State* L);
		static int RotateText(lua_State* L);
		static int FlipTextHorizental(lua_State* L);
		static int FlipTextVertical(lua_State* L);
		static int GetTextSize(lua_State* L);
		static int SetTextVisisble(lua_State* L);
		static int IsTextVisisble(lua_State* L);

		static int AttachTextTo(lua_State* L);
	private:
		static LuaText* _LUA_TEXT_;
	};
}

