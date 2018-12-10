#pragma once
#include <core/Common/defines.h>
#include <vector>
#include <functional>
#include <core/Physics/PhysicsEngine.h>
#include <core/Common/Macros.hpp>

class EventManager
{
public:
	static EventManager* GetEventManager();

	REGISTER_EVENT(ON_KEYBOARD_INPUT, void(int, unsigned int));
	REGISTER_EVENT(ON_MOUSE_CLICK, void(unsigned int, Uint8, const Vec2i&));
	REGISTER_EVENT(ON_MOUSE_MOVE, void(const Vec2i&));
	REGISTER_EVENT(ON_ENGINE_LOAD, void());
	REGISTER_EVENT(ON_ENGINE_PRE_INIT, void());
	REGISTER_EVENT(ON_ENGINE_RENDER, void(float));
	REGISTER_EVENT(ON_COLLISION_START, void(class b2Contact*));
	REGISTER_EVENT(ON_COLLISION_END, void(class b2Contact*));
	REGISTER_EVENT(ON_COLLISION_POST_SOLVE, void(class b2Contact*));
	REGISTER_EVENT(ON_COLLISION_PRE_SOLVE, void(class b2Contact*, const struct b2Manifold*));

	void HandleSDLEvents(SDL_Event&, class EngineInstance*);
	void HandleOnEnginePreInitEvents();
	void HandleOnEngineRenderEvents(float);
	void HandleOnEngineLoadEvents();

	friend class PhysicsContactListener;
private:
	EventManager(){}

	static EventManager* _EventManager;
	friend class UI::Base;
};