#pragma once

struct lua_State;

namespace LuaEngine {
	class LuaSprite
	{
	public:
		static void LoadSpriteSystem();
		static LuaSprite* GetSpriteManager();

		static void LoadSpriteFunction();

		static int CreateSprite(lua_State *L);

		static int SetSpritePosition(lua_State* L);
		static int SetSpriteRotationCenter(lua_State* L);
		static int GetSpritePosition(lua_State* L);
		static int GetSpriteFilename(lua_State* L);
		static int GetSpriteRotation(lua_State* L);

		static int DeleteSprite(lua_State* L);
		static int RotateSprite(lua_State* L);
		static int FlipSpriteHorizental(lua_State* L);
		static int FlipSpriteVertical(lua_State* L);
		static int GetSpriteSize(lua_State* L);
		static int SetSpriteVisisble(lua_State* L);
		static int IsSpriteVisisble(lua_State* L);

		static int Physicalize(lua_State* L);
		static int PhysicalizeWithOffset(lua_State* L);
		
		static int AttachSpriteTo(lua_State* L);
	private:
		static LuaSprite* _LUA_SPRITE_;
	};
}