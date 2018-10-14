#pragma once
#include <core/Common/defines.h>
#include <vector>
#include <functional>

class EventManager
{
public:
	typedef std::function<void(int, unsigned int)> InputFuncType;
	typedef std::function<void(unsigned int, Uint8, Vec2i)> MouseClickFuncType;
	typedef std::function<void(Vec2i)> MouseMoveFuncType;
	typedef std::function<void(class b2Contact*)> CollisionFuncType;
	typedef std::function<void(class b2Contact*, const struct b2Manifold*)> PreSolveCollisionFuncType;
	typedef std::function<void()> VoidFuncType;
	typedef std::function<void(float)> RenderFuncType;

	static EventManager* GetEventManager();

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_KEYBOARD_INPUT, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_InputCallbacks.push_back(InputFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_MOUSE_CLICK, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_MouseClickCallbacks.push_back(MouseClickFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_MOUSE_MOVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_MouseMoveCallbacks.push_back(MouseMoveFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_START, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionStartCallbacks.push_back(CollisionFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_END, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionEndCallbacks.push_back(CollisionFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_POST_SOLVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionPostSolveCallbacks.push_back(CollisionFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_PRE_SOLVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionPreSolveCallbacks.push_back(PreSolveCollisionFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_LOAD, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_OnEngineLoadCallbacks.push_back(VoidFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_PRE_INIT, bool>::type = true>
	void addEventHandler(Func&& func)
	{
		m_OnEnginePreInitCallbacks.push_back(VoidFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_RENDER, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_OnEnginePreInitCallbacks.push_back(RenderFuncType(std::forward<Func>(func)));
	}

	void HandleSDLEvents(SDL_Event&, class EngineInstance*);
	void HandleOnEnginePreInitEvents();
	void HandleOnEngineRenderEvents(float);

	void HandleOnEngineLoadEvents();

	friend class PhysicsContactListener;
private:
	EventManager(){}

	//Input callbacks
	std::vector<InputFuncType> m_InputCallbacks;

	// Mouse call backs
	std::vector<MouseClickFuncType> m_MouseClickCallbacks;
	std::vector<MouseMoveFuncType> m_MouseMoveCallbacks;

	//Physics callback
	std::vector<CollisionFuncType> m_OnCollisionStartCallbacks;
	std::vector<CollisionFuncType> m_OnCollisionEndCallbacks;
	std::vector<CollisionFuncType> m_OnCollisionPostSolveCallbacks;
	std::vector<PreSolveCollisionFuncType> m_OnCollisionPreSolveCallbacks;

	//ON ENGINE EVENTS!
	std::vector<VoidFuncType> m_OnEngineLoadCallbacks;
	std::vector<VoidFuncType> m_OnEnginePreInitCallbacks;
	std::vector<RenderFuncType> m_OnEngineRenderCallbacks;

	static EventManager* _EventManager;
};