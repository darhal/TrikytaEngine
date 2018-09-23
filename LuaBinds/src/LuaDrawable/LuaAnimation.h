#pragma once

struct lua_State;

namespace LuaEngine
{
	class LuaAnimation
	{
	public:
		static void LoadAnimationSystem();
		static LuaAnimation* GetAnimationManager();
		static void LoadAnimationFunctions();

		static int CreateAnimation(lua_State*);
		static int SetAnimationPosition(lua_State* L);
		static int SetAnimationRotationCenter(lua_State* L);
		static int GetAnimationPosition(lua_State* L);
		static int GetAnimationFilename(lua_State* L);

		static int DeleteAnimation(lua_State* L);
		static int RotateAnimation(lua_State* L);
		static int FlipAnimationHorizental(lua_State* L);
		static int FlipAnimationVertical(lua_State* L);
		static int GetAnimationSize(lua_State* L);

		static int SetAnimationVisisble(lua_State* L);
		static int IsAnimationVisisble(lua_State* L);
		static int GetAnimationRotation(lua_State* L);
		static int AttachAnimationTo(lua_State* L);
	private:
		static LuaAnimation* _LUA_ANIMATION_;
	};
}