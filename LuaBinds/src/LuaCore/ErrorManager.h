#pragma once
#include <string>

struct lua_State;

class ErrorManager
{
public:
	static ErrorManager* InitErrorManager();
	static ErrorManager* GetErrorManager();
	bool CheckType(lua_State*, int, std::string, int = 0);
	int GetLine(lua_State*, int);
	const char* GetFunction(lua_State* , int);
	const char* GetFile(lua_State*, int);
	bool ArgumentCheckerHelper(lua_State*, const char*, ...);
	bool isValidArgument(lua_State*, const char*);
	static const char* LuaTypesMap[];
private:
	static ErrorManager* _ErrorManager;
};
