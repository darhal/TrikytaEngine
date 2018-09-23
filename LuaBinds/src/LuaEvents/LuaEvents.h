#pragma once
#include <map>
#include <core/Events/EventManager.h>

struct lua_State;

namespace LuaFunctions 
{
	int addEventHandler(struct lua_State*);
}

namespace LuaEngine {
	

	class LuaEvents
	{
	public:
		static LuaEvents* GetLuaEventMnager();
		void RegisterLuaEventManager();
		void RegisterCallBack(Events, int);
		void CallLuaEventFunctions(Events, const char *, ...);
	private:
		
		static LuaEvents* LuaEventManager;
		std::map<Events, std::vector<int>> m_LuaCallBackFunction;
	};

}