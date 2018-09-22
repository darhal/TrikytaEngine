#pragma once
#include "Vec2.h"

class Utility
{
public:
	static char* getDateNow();
	template<typename T>
	static T ToMeters(T x);
	template<typename T>
	static T ToPixels(T x);
	template<typename T>
	static T ToRadian(T x);
	template<typename T>
	static T ToDegree(T x);
	static Vec2f ConvertWorldToScreen(Vec2f);
	static Vec2f ConvertScreenToWorld(Vec2f);
};