#include "LStateManager/LStateManager.h"
#include "LuaEvents.h"
#include <lua.hpp>
#include "Shared/ldefines.h"

using namespace LuaEngine;

LuaEvents* LuaEvents::LuaEventManager = nullptr;

int LuaFunctions::addEventHandler(lua_State* L)
{
	luaL_checktype(L, -1, LUA_TFUNCTION);
	int CallBackRefFunc = luaL_ref(L, LUA_REGISTRYINDEX);
	const char* _EventType = lua_tostring(L, 1);
	
	if (strcmp(_EventType, "OnKeyboardInput") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_KEYBOARD_INPUT, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnCollisionStart") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_COLLISION_START, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnCollisionEnd") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_COLLISION_END, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnMouseClick") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_MOUSE_CLICK, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnMouseMove") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_MOUSE_MOVE, CallBackRefFunc);
	}else if (strcmp(_EventType , "OnEngineLoad") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_ENGINE_LOAD, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnEnginePreInit") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_ENGINE_PRE_INIT, CallBackRefFunc);
	}else if (strcmp(_EventType, "OnEngineRender") == 0) {
		LuaEvents::GetLuaEventMnager()->RegisterCallBack(Events::ON_ENGINE_RENDER, CallBackRefFunc);
	}
	return 0;
}

void LuaEvents::RegisterCallBack(Events p_Event, int p_FuncIndex)
{
	m_LuaCallBackFunction[p_Event].emplace_back(p_FuncIndex);
}

void LuaEvents::CallLuaEventFunctions(Events p_Event, const char *sig, ...)
{
	if (m_LuaCallBackFunction[p_Event].empty()) {return;}
	auto L = LStateManager::GetLuaState();
	for (auto IndexFuncToCall : m_LuaCallBackFunction[p_Event]) {
		va_list vl;
		int narg, nres;  //number of arguments and results 
		va_start(vl, sig);
		lua_rawgeti(L, LUA_REGISTRYINDEX, IndexFuncToCall);
		narg = 0;
		while (*sig) {  //push arguments 
			switch (*sig++) {

			case 'd':  // double argument
				lua_pushnumber(L, va_arg(vl, double));
				break;

			case 'i':  //int argument 
				lua_pushnumber(L, va_arg(vl, int));
				break;

			case 's':  //string argument 
				lua_pushstring(L, va_arg(vl, char *));
				break;

			case '>':
				goto endwhile;

			default:
				LogL("ERROR", "invalid option (%c)", *(sig - 1));
			}
			narg++;
			
			luaL_checkstack(L, 1, "too many arguments");
		} endwhile:
		nres = strlen(sig); //Num of res coming back
		if (lua_pcall(L, narg, nres, 0)) 
		{
			LogL("ERROR", "Attempt running function : %s", lua_tostring(L, -1));
		}
		va_end(vl);
	}
	
}

LuaEvents* LuaEvents::GetLuaEventMnager()
{
	if (LuaEventManager == nullptr)
	{
		LuaEventManager = new LuaEvents();
	}
	return LuaEventManager;
}

void LuaEvents::RegisterLuaEventManager()
{
	LogL("INFO", "Registering Lua Event System!")
	auto L = LStateManager::GetLuaState();
	lua_pushcfunction(L, LuaFunctions::addEventHandler);
	lua_setglobal(L, "AddEventHandler");
}