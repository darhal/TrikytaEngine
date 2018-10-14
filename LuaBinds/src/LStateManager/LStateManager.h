#pragma once
#include <string>
#include <memory>
#include <vector>

struct lua_State;
struct ENGINE_CONFIG;

namespace LuaEngine {
	class LStateManager
	{
	public:
		static LStateManager* GetLStateManager();
		static ::lua_State* GetLuaState();

		void LoadScripts();
		::ENGINE_CONFIG LLoadConfig(::lua_State* L);
		std::shared_ptr<std::vector<std::string>> LoadScriptList();
		void LoadingTrikytaEnv();
		void RestertScript(std::string);
	private:
		LStateManager();
		static LStateManager* _Lua_Engine;
		::lua_State* _LUA_STATE_;
	};
}