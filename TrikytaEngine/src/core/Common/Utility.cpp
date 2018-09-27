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

void Utility::LogVaradicConsole(MESSAGE_TYPE msg_type, const char* format, ...)
{
	char buffer[256] = { 0 };
	va_list argptr;
	va_start(argptr, format);
	vsnprintf(buffer, 255, format, argptr);
	va_end(argptr);
	Console::getConsole()->outputConsole(buffer, msg_type);
}

int Utility::ToMeters(int x)
{
	return (int)(x / PTM);
}

int Utility::ToPixels(int x)
{
	return (int)(x * PTM);
}

int Utility::ToRadian(int x)
{
	return (int)(DEGTORAD * x);
}

int Utility::ToDegree(int x)
{
	return (int)(RADTODEG * x);
}


float Utility::ToMeters(float x)
{
	return (float)(x / PTM);
}

float Utility::ToPixels(float x)
{
	return (float)(x * PTM);
}

float Utility::ToRadian(float x)
{
	return (float)(DEGTORAD * x);
}

float Utility::ToDegree(float x)
{
	return (float)(RADTODEG * x);
}