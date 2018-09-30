#pragma once
#include <string>
#include <lua.hpp>
#include <misc/Console.h>

struct lua_State;

/*struct Variable
{
	Variable()
		: data(NULL)
	{
	}

	template <typename T>
	Variable(T* data_)
		: data(data_)
	{
	}

	Variable(void* data_)
		: data(data_)
	{
	}

	template <typename T>
	T& Get(void)
	{
		return *((T *)data);
	}

	template <typename T>
	T Get(bool)
	{
		return ((T)data);
	}

	template <typename T>
	Variable& operator=(T& rhs)
	{
		data = &rhs;

		return *this;
	}

	void* data;
};*/

class ErrorManager
{
public:
	static ErrorManager* InitErrorManager();
	static ErrorManager* GetErrorManager();
	bool CheckType(lua_State*, int, std::string, int = 0);
	int GetLine(lua_State*, int);
	const char* GetFunction(lua_State* , int);
	const char* GetFile(lua_State*, int);
	bool isValidArgument(lua_State*, const char*);
	//std::vector <std::variant<std::any>> m_Args;

	/*template<typename FirstArg, typename... Targs>
	void ParseArgs(lua_State* L,const char* types, Variable* Args, FirstArg value, Targs... Fargs) // recursive variadic function
	{
		int ArgIndex = strlen(types);
		//LogConsole(MESSAGE_TYPE::INFO, "ARG INDEX IS %d", ArgIndex);
		if (*types) {
			switch (*types++) {
			case 'b':
				//m_Args.emplace_back(m_Args, (bool)lua_toboolean(L, ArgIndex));
				Args[ArgIndex - 1] = (void*)((((bool)lua_toboolean(L, ArgIndex))));
				//LogConsole(MESSAGE_TYPE::INFO, "BOOL AT %d PTR=%p", ArgIndex, ((bool)Args[ArgIndex - 1].data));
				break;
			case 'l':
				//m_Args.emplace_back(m_Args, (FirstArg*)(lua_touserdata(L, ArgIndex)));
				Args[ArgIndex - 1] = (lua_touserdata(L, ArgIndex));
				//LogConsole(MESSAGE_TYPE::INFO, "LIGH USER DATA AT %d PTR=%p", ArgIndex, Args[ArgIndex - 1]);
				break;
			case 'n':
				//m_Args.emplace_back(m_Args, lua_tonumber(L, ArgIndex));
				Args[ArgIndex - 1] = (void*)(new double(lua_tonumber(L, ArgIndex)));
				break;
			case 's':
				//m_Args.emplace_back(m_Args, std::string(lua_tostring(L, ArgIndex)));
				Args[ArgIndex - 1] = (void*)(new std::string(lua_tostring(L, ArgIndex)));
				break;
			case 't':

				break;
			case 'f':

				break;
			case 'u':

				break;

			default:
				break;
			}
			ParseArgs(L, types, Args++, Fargs...);
		}
	}*/
	
	static const char* LuaTypesMap[];
private:
	static ErrorManager* _ErrorManager;
};

/*

/*
  Copyright (c) 2013 Randy Gaul http://RandyGaul.net

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

Creator(s)    : Randy Gaul
Creation Date : Dec 29 2013
File Name     : main.cpp
Purpose       : Demonstration of compile-time generic function binding.
Note(s)       : Many limitations in this demo:
				- No type safety
				- No support for return values
				- No class/struct method support
				- Unable to enumerate return value or argument types
				  during runtime. For methods, unable to enumerate the
				  type of the this pointer.
				- Variable does not work properly with char *, and
				  possibly other pointer types.

				All limitations can be solved with some more templates
				and some custom introspection work.




template <typename FunctionType, FunctionType FunctionPtr>
void Call(Variable *args)
{
	(*FunctionPtr)();
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1>
void Call(Variable *args)
{
	(*FunctionPtr)(
		args[0].Get<Arg1>()
		);
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2>
void Call(Variable *args)
{
	(*FunctionPtr)(
		args[0].Get<Arg1>(),
		args[1].Get<Arg2>()
		);
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3>
void Call(Variable *args)
{
	(*FunctionPtr)(
		args[0].Get<Arg1>(),
		args[1].Get<Arg2>(),
		args[2].Get<Arg3>()
		);
}

struct Function
{
	Function(
		void(*fn)(void),
		void(*helper)(Variable *)
	)
		: callHelper(helper)
	{
	}

	template <typename Arg1>
	Function(
		void(*fn)(Arg1),
		void(*helper)(Variable *)
	)
		: callHelper(helper)
	{
	}

	template <typename Arg1, typename Arg2>
	Function(
		void(*fn)(Arg1, Arg2),
		void(*helper)(Variable *)
	)
		: callHelper(helper)
	{
	}

	void operator( )(void)
	{
		callHelper(NULL);
	}

	template <typename Arg1>
	void operator( )(Arg1 arg1)
	{
		Variable args[] = {
		  arg1
		};

		callHelper(args);
	}

	template <typename Arg1, typename Arg2>
	void operator( )(Arg1 arg1, Arg2 arg2)
	{
		Variable args[] = {
		  arg1,
		  arg2
		};

		callHelper(args);
	}

	template <typename Arg1, typename Arg2, typename Arg3>
	void operator( )(Arg1 arg1, Arg2 arg2, Arg3 arg3)
	{
		Variable args[] = {
		  arg1,
		  arg2,
		  arg3,
		};

		callHelper(args);
	}

	//void(*callHelper)(Variable *);
	std::function<void(Variable *)> callHelper;
};

template <typename FunctionType, FunctionType FunctionPtr>
Function BuildFunction(void(*fn)(void))
{
	return Function(fn, &Call<FunctionType, FunctionPtr>);
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1>
Function BuildFunction(void(*fn)(Arg1))
{
	return Function(fn, &Call<FunctionType, FunctionPtr, Arg1>);
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2>
Function BuildFunction(void(*fn)(Arg1, Arg2))
{
	return Function(fn, &Call<FunctionType, FunctionPtr, Arg1, Arg2>);
}

template <typename FunctionType, FunctionType FunctionPtr, typename Arg1, typename Arg2, typename Arg3>
Function BuildFunction(void(*fn)(Arg1, Arg2, Arg3))
{
	return Function(fn, &Call<FunctionType, FunctionPtr, Arg1, Arg2, Arg3>);
}

#define FUNCTION( FN ) \
  BuildFunction<decltype( &FN ), &FN>( &FN )

  */