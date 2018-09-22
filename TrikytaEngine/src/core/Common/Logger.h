#pragma once
#include "Utility.h"

#ifdef __DEBUG__
	#define Log(...) printf("[TrikytaEngine-%s]: ", Utility::getDateNow()); printf(__VA_ARGS__); printf("\n");
#else
	#define Log(...) ;
#endif
