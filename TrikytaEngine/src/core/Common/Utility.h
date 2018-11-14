#pragma once
#include "Vec2.h"
#include <string>

enum class MESSAGE_TYPE;

class Utility
{
public:
	static std::string getDateNow();

	static float ToMeters(float x);
	static float ToPixels(float x);
	static float ToRadian(float x);
	static float ToDegree(float x);

	static int ToMeters(int x);
	static int ToPixels(int x);
	static int ToRadian(int x);
	static int ToDegree(int x);

	template<typename T>
	static bool IsInBox(Vec2<T> pos, Vec2<T> minPos, Vec2<T> maxPos)
	{
		return (pos.x >= minPos.x && pos.y >= minPos.y) && (pos.x <= maxPos.x && pos.y <= maxPos.y);
	}


	static void LogVaradicConsole(MESSAGE_TYPE, const char* format, ...);
};
