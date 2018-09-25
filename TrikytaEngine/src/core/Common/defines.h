#pragma once
#define __DEBUG__			 1
#include "Logger.h"

#define SDL_main				main
#define DEFAULT_WND_NAME		"Trikyta Engine"
#define ASSUMED_MAX_OBJECTS		50
#define ENGINE					TrikytaEngine::getEngine()
#define DEGTORAD				0.0174532925199432957f
#define RADTODEG				57.295779513082320876f
#define FREE(p)					do { delete (p); (p) = 0; } while(0)//if (x != NULL) { delete x; x=NULL;};
/** We need this to easily convert between pixel and real-world coordinates*/
static const float PTM =		2.f;

namespace Physics2D {
	enum class BodyType;
	enum class BodyShape;
	struct BodyParams;
	struct PhysicsEngineParams;
	class PhysicsEngine;
	class PhysicsWorld;
	class PhysicsBody;
};

#define CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)