#pragma once

struct lua_State;

namespace LuaEngine {
	class LuaBody
	{
	public:
		static void LoadPhysicsBodySystem();
		static LuaBody* GetBodyManager();

		static void LoadBodyFunction();

		static int CreateBody(lua_State *L);
		static int SetLinearVelocity(lua_State *L);
		static int SetAngularDamping(lua_State* L);
		static int GetLinearVelocity(lua_State* L);
		static int GetAngularDamping(lua_State* L);
	private:
		static LuaBody* _LUA_BODY_;
	};
}