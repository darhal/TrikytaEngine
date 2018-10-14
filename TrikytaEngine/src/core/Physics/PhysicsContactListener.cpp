#include "PhysicsContactListener.h"
#include <core/Events/EventManager.h>

PhysicsContactListener* PhysicsContactListener::_PhysicsContactListener = 0;

PhysicsContactListener* PhysicsContactListener::GetPhysicsContactListener()
{
	if (_PhysicsContactListener == nullptr)
	{
		_PhysicsContactListener = new PhysicsContactListener();
	}
	return _PhysicsContactListener;
}

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
	for (auto onContactBegin : EventManager::GetEventManager()->m_OnCollisionStartCallbacks) {
		onContactBegin(contact);
	}
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
	for (auto onContactEnd : EventManager::GetEventManager()->m_OnCollisionEndCallbacks) {
		onContactEnd(contact);
	}
}

void PhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	for (auto onPreSolve : EventManager::GetEventManager()->m_OnCollisionPreSolveCallbacks) {
		onPreSolve(contact, oldManifold);
	}
}

void PhysicsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	for (auto onPostSolve : EventManager::GetEventManager()->m_OnCollisionPostSolveCallbacks) {
		onPostSolve(contact);
	}
}