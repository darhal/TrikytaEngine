#pragma once
#include <string>
#include <memory>
#include <vector>

struct lua_State;

namespace LuaEngine {
	struct WND_CONFIG
	{
		int w;
		int h;
		const char* name;
	};

	class LStateManager
	{
	public:
		static LStateManager* GetLStateManager();
		static ::lua_State* GetLuaState();

		void LoadScripts();
		WND_CONFIG LLoadConfig(::lua_State* L);
		std::shared_ptr<std::vector<std::string>> LoadScriptList();
		void LoadingTrikytaEnv();
	private:
		LStateManager();
		static LStateManager* _Lua_Engine;
		::lua_State* _LUA_STATE_;
	};
}