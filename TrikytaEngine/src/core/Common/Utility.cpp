#include <stdio.h>
#include <chrono>
#include <ctime>
#include "TrikytaEngine.h"
#include "EngineInstance.h"
#include "Utility.h"

#pragma warning(disable:4996) // DISABLE STUPID VS ERRORS

char* Utility::getDateNow()
{
	auto time = std::chrono::system_clock::now();
	std::time_t log_time = std::chrono::system_clock::to_time_t(time);
	char* date = std::ctime(&log_time);
	date[strlen(date) - 1] = '\0';
	return date;
}

template<typename T>
T Utility::ToMeters(T x) 
{

}

template<typename T>
T Utility::ToPixels(T x) 
{

}

template<typename T>
T Utility::ToRadian(T x) 
{
	return DEGTORAD * x;
}

template<typename T>
T Utility::ToDegree(T x) 
{
	return RADTODEG * x;
}

Vec2f Utility::ConvertWorldToScreen(Vec2f pw)
{
	// CODE TAKEN FROM HERE: http://www.box2d.org/forum/viewtopic.php?t=8198
	float offsetX = ENGINE->GetScreenWeight()/PTM; //x offset in meters (400/50 = 8). This will put the 0 x-coordinate in the middle of the screen horizontally.
	float offsetY = ENGINE->GetScreenHeight()/PTM; //y offset in meters (300/50 = 6). This will put the 0 y-coordinate in the middle of the screen vertically.
	float drawPositionX = (pw.x*PTM + offsetX); //((0m) +  8.0m)*50 = 400 pixels
	float drawPositionY = (pw.y*PTM + offsetY); //(-(4m) + 6.0m)*50 = 100 pixels
	return Vec2f(drawPositionX, drawPositionY);
}

Vec2f Utility::ConvertScreenToWorld(Vec2f ps)
{
	float offsetX = ENGINE->GetScreenWeight()/(2*PTM);
	float offsetY = ENGINE->GetScreenHeight()/(2*PTM);
	float WorldX = (ps.x / PTM) - offsetX;
	float WorldY = (ps.y / PTM) - offsetY;
	//return b2Vec2(WorldX, WorldY);
	return Vec2f(WorldX, WorldY);
}