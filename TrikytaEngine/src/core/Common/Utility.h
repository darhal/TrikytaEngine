#pragma once
#include "Vec2.h"

class Utility
{
public:
	static char* getDateNow();

	static float ToMeters(float x);
	static float ToPixels(float x);
	static float ToRadian(float x);
	static float ToDegree(float x);

	static int ToMeters(int x);
	static int ToPixels(int x);
	static int ToRadian(int x);
	static int ToDegree(int x);

	static void LogVaradicConsole(enum class MESSAGE_TYPE, const char* format, ...);
};