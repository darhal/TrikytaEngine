#include "ErrorManager.h"
#include "LStateManager/LStateManager.h"
#include <lua.hpp>
#include <core/Common/defines.h>
#include <map>

const char* ErrorManager::LuaTypesMap[] = {"nil", "bool", "lightuserdata", "number", "string", "table", "function", "userdata", "thread"};

ErrorManager* ErrorManager::_ErrorManager = nullptr;

ErrorManager* ErrorManager::InitErrorManager()
{
	if (_ErrorManager != nullptr){
		_ErrorManager = new ErrorManager();
	}
	return _ErrorManager;
}

ErrorManager* ErrorManager::GetErrorManager()
{
	return _ErrorManager;
}

int ErrorManager::GetLine(lua_State* L, int level) 
{
	lua_Debug ar;
	lua_getstack(L, level, &ar);
	lua_getinfo(L, "l", &ar);
	return ar.currentline;
}

const char* ErrorManager::GetFunction(lua_State* L, int level) // TODO: fix this
{
	lua_Debug ar;
	lua_getstack(L, level, &ar);
	lua_getinfo(L, "n", &ar);
	return ar.name;
}

const char* ErrorManager::GetFile(lua_State* L, int level)
{
	lua_Debug ar;
	lua_getstack(L, level, &ar);
	lua_getinfo(L, "S", &ar);
	return ar.source;
}

bool ErrorManager::CheckType(lua_State* L, int p_StackIndex, std::string p_TypeToCheck, int p_ArgNum)
{
	if (p_ArgNum == 0)
		p_ArgNum = p_StackIndex;
	if (LuaTypesMap[lua_type(L, p_StackIndex)] != p_TypeToCheck) {
		//luaL_argerror(L, p_ArgNum, "");
		LogConsole(MESSAGE_TYPE::ERROR, "%s Line %d: Expected a %s at \nargument #%d of function " LUA_QS " got %s",
			GetFile(L, 1), GetLine(L, 1), p_TypeToCheck.c_str(),p_ArgNum, GetFunction(L, 0), LuaTypesMap[lua_type(L, p_StackIndex)]);
		return true;
	}
	return false;
}

bool ErrorManager::isValidArgument(lua_State* L, const char* sig)
{
	int arg = 1;
	bool shouldContinue = true;
	while (*sig && shouldContinue) {
		switch (*sig++) {
		case 'b':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "bool")) {
				shouldContinue = false;
			}
			break;
		case 'l':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "lightuserdata")) {
				shouldContinue = false;
			}
			break;
		case 'n':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "number")) {
				shouldContinue = false;
			}
			break;
		case 's':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "string")) {
				shouldContinue = false;
			}
			break;
		case 't':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "table")) {
				shouldContinue = false;
			}
			break;
		case 'f':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "function")) {
				shouldContinue = false;
			}
			break;
		case 'u':
			if (ErrorManager::GetErrorManager()->CheckType(L, arg, "userdata")) {
				shouldContinue = false;
			}
			break;
		case '>':
			goto endwhile;

		default:
			break;
		}
		arg++;

	} endwhile:
	if (!shouldContinue) {
		lua_pushnil(L);
	}
	return shouldContinue;
}

