#pragma once
#include <string>
#include <memory>
#include <vector>

struct lua_State;

namespace LuaBinds
{
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
	static int DeletSprite(lua_State* L);
	void OnRender(float dt);
}

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