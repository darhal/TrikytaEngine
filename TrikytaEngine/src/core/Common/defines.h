#pragma once
#define __DEBUG__			 1

#define SDL_main				main
#define DEFAULT_WND_NAME		"Trikyta Engine"
#define ASSUMED_MAX_OBJECTS		50
#define ENGINE					TrikytaEngine::getEngine()
#define DEGTORAD				0.0174532925199432957f
#define RADTODEG				57.295779513082320876f
#define	MAX_CONSOLE_OUPUT		5
#define FREE(p)					do { delete (p); p = NULL;} while(0)//if (x != NULL) { delete x; x=NULL;};

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

namespace UI {
	class Text;
	class EditBox;
}

struct ENGINE_CONFIG
{
	ENGINE_CONFIG
	(
		unsigned int pWINDOW_WIDTH = (unsigned int)(1024 * 1.2),
		unsigned int pWINDOW_HEIGHT = (unsigned int)(768 * 1.2),
		const char* pWINDOW_NAME = "Trikyta Engine",
		unsigned int pACCELERATION_RATE = 1, 
		bool pPHYSICS_DEBUG = true, 
		float pPHYSICS_TIME_STEP = 1/60, 
		int pPHYSICS_VELOCITY_ITERATIONS = 8, 
		int pPHYSICS_POSITION_ITERATIONS = 3, 
		float pPHYSICS_WORLD_GRAVITY_X = 0, 
		float pPHYSICS_WORLD_GRAVITY_Y = 10
	)
		:
		WINDOW_WIDTH(pWINDOW_WIDTH),
		WINDOW_HEIGHT(pWINDOW_HEIGHT),
		WINDOW_NAME(pWINDOW_NAME),
		ACCELERATION_RATE(pACCELERATION_RATE),
		PHYSICS_DEBUG(pPHYSICS_DEBUG),
		PHYSICS_TIME_STEP(pPHYSICS_TIME_STEP),
		PHYSICS_VELOCITY_ITERATIONS(pPHYSICS_VELOCITY_ITERATIONS),
		PHYSICS_POSITION_ITERATIONS(pPHYSICS_POSITION_ITERATIONS),
		PHYSICS_WORLD_GRAVITY_X(pPHYSICS_WORLD_GRAVITY_X),
		PHYSICS_WORLD_GRAVITY_Y(pPHYSICS_WORLD_GRAVITY_Y)
	{}

	unsigned int WINDOW_WIDTH		= (unsigned int)(1024 * 1.2);
	unsigned int WINDOW_HEIGHT		= (unsigned int)(768 * 1.2);
	const char*	WINDOW_NAME			= "Trikyta Engine";
	unsigned int ACCELERATION_RATE	= 1;
	bool PHYSICS_DEBUG				= true;
	float PHYSICS_TIME_STEP			= 1 / 60;
	int PHYSICS_VELOCITY_ITERATIONS = 8;
	int PHYSICS_POSITION_ITERATIONS = 3;
	float PHYSICS_WORLD_GRAVITY_X	= 0;
	float PHYSICS_WORLD_GRAVITY_Y	= 10;
};

enum Events
{
	//INPUT EVENTS
	ON_KEYBOARD_INPUT = 1,
	ON_MOUSE_CLICK = 2,
	ON_MOUSE_MOVE = 3,

	//Physics EVENTS
	ON_COLLISION_START = 4,
	ON_COLLISION_END = 5,
	ON_COLLISION_PRE_SOLVE = 6,
	ON_COLLISION_POST_SOLVE = 7,

	//Engine EVENTS
	ON_ENGINE_LOAD = 8,
	ON_ENGINE_PRE_INIT = 9,
	ON_ENGINE_RENDER = 10,

	//UI EVENTS
	ON_EDITBOX_CHANGE = 11,
};

#define CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)
#define CALLBACK_5(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)
#define CALLBACK_6(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,std::placeholders::_6,##__VA_ARGS__)

typedef signed char        int8;
typedef short              int16;
typedef int                int32;
typedef long long          int64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef signed char        int_least8;
typedef short              int_least16;
typedef int                int_least32;
typedef long long          int_least64;
typedef unsigned char      uint_least8;
typedef unsigned short     uint_least16;
typedef unsigned int       uint_least32;
typedef unsigned long long uint_least64;

typedef signed char        int_fast8;
typedef int                int_fast16;
typedef int                int_fast32;
typedef long long          int_fast64;
typedef unsigned char      uint_fast8;
typedef unsigned int       uint_fast16;
typedef unsigned int       uint_fast32;
typedef unsigned long long uint_fast64;

typedef long long          intmax;
typedef unsigned long long uintmax;

#include "Logger.h"
