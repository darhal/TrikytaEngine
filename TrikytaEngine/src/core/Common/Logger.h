#pragma once
#include <cstdarg>
#include "Utility.h"
#include "misc/Console.h"
#pragma warning(disable:4996) // DISABLE STUPID VS ERRORS


#ifdef __DEBUG__
	#define LogTerminal(...) printf("[TrikytaEngine-%s]: ", Utility::getDateNow()); printf(__VA_ARGS__); printf("\n");
	#define Log(...) printf("[TrikytaEngine-%s]: ", Utility::getDateNow()); printf(__VA_ARGS__); printf("\n");
	#define LogInfoConsole(__format__, ...) Utility::LogVaradicConsole(MESSAGE_TYPE::INFO, __format__, __VA_ARGS__);
	#define LogConsole(_MSG_TYPE_, __format__,...) Utility::LogVaradicConsole(_MSG_TYPE_, __format__, __VA_ARGS__);
#else
	#define LogTerminal(...) ;
#endif


