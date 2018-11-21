#pragma once
#include <cstdarg>
#include "Utility.h"
#include "misc/Console/Console.h"

#ifdef _MSC_VER
#pragma warning(disable:4996) // DISABLE STUPID VS ERRORS
#endif


#ifdef __DEBUG__
	#define LogTerminalFromConsole(...) printf("[%s][TrikytaEngine]", Utility::getDateNow().c_str()); printf(__VA_ARGS__); printf("\n");
	#define LogTerminal(...) printf("[%s][TrikytaEngine]: ", Utility::getDateNow().c_str()); printf(__VA_ARGS__); printf("\n");
	#define Log(...) printf("[%s][TrikytaEngine]: ", Utility::getDateNow().c_str()); printf(__VA_ARGS__); printf("\n");
	#define LogInfoConsole(__format__, ...) Utility::LogVaradicConsole(MESSAGE_TYPE::INFO, __format__, ##__VA_ARGS__);
	#define LogConsole(_MSG_TYPE_, __format__,...) Utility::LogVaradicConsole(_MSG_TYPE_, __format__, ##__VA_ARGS__);
#else
	#define LogTerminal(...) ;
#endif

#define LogInfo MESSAGE_TYPE::INFO
#define LogWarning MESSAGE_TYPE::WARNING
#define LogError MESSAGE_TYPE::ERROR