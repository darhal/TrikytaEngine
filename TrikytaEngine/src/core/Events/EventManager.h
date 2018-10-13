#pragma once
#include <core/Common/defines.h>
#include <SDL/SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>


enum Events
{
	ON_KEYBOARD_INPUT = 1,

	ON_MOUSE_CLICK = 2,
	ON_MOUSE_MOVE = 3,

	ON_COLLISION_START = 4,
	ON_COLLISION_END = 5,
	ON_COLLISION_PRE_SOLVE = 6,
	ON_COLLISION_POST_SOLVE = 7,

	ON_ENGINE_LOAD = 8,
	ON_ENGINE_PRE_INIT = 9,
	ON_ENGINE_RENDER = 10,

	ON_EDITBOX_CHANGE = 11,
};

class EventManager
{
public:
	typedef std::function<void(SDL_Keycode, unsigned int)> InputFuncType;
	typedef std::function<void(unsigned int, Uint8, Vec2i)> MouseClickFuncType;
	typedef std::function<void(Vec2i)> MouseMoveFuncType;
	typedef std::function<void(class b2Contact*)> CollisionFuncType;
	typedef std::function<void(class b2Contact*, const struct b2Manifold*)> PreSolveCollisionFuncType;
	typedef std::function<void()> VoidFuncType;
	typedef std::function<void(float)> RenderFuncType;
	typedef std::function<void(SDL_Event&)> OnEditBoxChangeType;

	static EventManager* GetEventManager();

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_KEYBOARD_INPUT, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_InputCallbacks->push_back(InputFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_MOUSE_CLICK, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_MouseClickCallbacks->push_back(MouseClickFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_MOUSE_MOVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_MouseMoveCallbacks->push_back(MouseMoveFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_START, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionStartCallbacks->push_back(CollisionFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_END, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionEndCallbacks->push_back(CollisionFuncType(std::forward<Func>(func)));
	}
	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_POST_SOLVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionPostSolveCallbacks->push_back(CollisionFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_COLLISION_PRE_SOLVE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		Physics2D::PhysicsEngine::GetPhysicsWorld()->AddContactListener();
		m_OnCollisionPreSolveCallbacks->push_back(PreSolveCollisionFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_LOAD, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_OnEngineLoadCallbacks->push_back(VoidFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_PRE_INIT, bool>::type = true>
	void addEventHandler(Func&& func)
	{
		m_OnEnginePreInitCallbacks->push_back(VoidFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_ENGINE_RENDER, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_OnEnginePreInitCallbacks->push_back(RenderFuncType(std::forward<Func>(func)));
	}

	template <Events EventType, typename Func,
		typename std::enable_if<EventType == ON_EDITBOX_CHANGE, bool>::type = true>
		void addEventHandler(Func&& func)
	{
		m_OnEditBoxChangeCallbacks->push_back(OnEditBoxChangeType(std::forward<Func>(func)));
	}

	void HandleSDLEvents(SDL_Event&, class EngineInstance*);
	void HandleOnEnginePreInitEvents();
	void HandleOnEngineRenderEvents(float);

	void HandleOnEngineLoadEvents();

	friend class PhysicsContactListener;
private:
	EventManager() :
		m_InputCallbacks(new std::vector<InputFuncType>),

		m_MouseClickCallbacks(new std::vector<MouseClickFuncType>),
		m_MouseMoveCallbacks(new std::vector<MouseMoveFuncType>),

		m_OnCollisionStartCallbacks(new std::vector<CollisionFuncType>),
		m_OnCollisionEndCallbacks(new std::vector<CollisionFuncType>),
		m_OnCollisionPostSolveCallbacks(new std::vector<CollisionFuncType>),
		m_OnCollisionPreSolveCallbacks(new std::vector<PreSolveCollisionFuncType>),

		m_OnEngineLoadCallbacks(new std::vector<VoidFuncType>),
		m_OnEnginePreInitCallbacks(new std::vector<VoidFuncType>),
		m_OnEngineRenderCallbacks(new std::vector<RenderFuncType>),
		m_OnEditBoxChangeCallbacks(new std::vector<OnEditBoxChangeType>)
	{}

	//Input callbacks
	std::vector<InputFuncType>* m_InputCallbacks;

	// Mouse call backs
	std::vector<MouseClickFuncType>* m_MouseClickCallbacks;
	std::vector<MouseMoveFuncType>* m_MouseMoveCallbacks;

	//Physics callback
	std::vector<CollisionFuncType>* m_OnCollisionStartCallbacks;
	std::vector<CollisionFuncType>* m_OnCollisionEndCallbacks;
	std::vector<CollisionFuncType>* m_OnCollisionPostSolveCallbacks;
	std::vector<PreSolveCollisionFuncType>* m_OnCollisionPreSolveCallbacks;

	//ON ENGINE EVENTS!
	std::vector<VoidFuncType>* m_OnEngineLoadCallbacks;
	std::vector<VoidFuncType>* m_OnEnginePreInitCallbacks;
	std::vector<RenderFuncType>* m_OnEngineRenderCallbacks;

	//UI Events:
	std::vector<OnEditBoxChangeType>* m_OnEditBoxChangeCallbacks;

	static EventManager* _EventManager;
};