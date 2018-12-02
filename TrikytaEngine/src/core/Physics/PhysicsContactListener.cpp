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
	for (auto onContactBegin : EventManager::GetEventManager()->m_ON_COLLISION_START_Callbacks) {
		onContactBegin(contact);
	}
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
	for (auto onContactEnd : EventManager::GetEventManager()->m_ON_COLLISION_END_Callbacks) {
		onContactEnd(contact);
	}
}

void PhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	for (auto onPreSolve : EventManager::GetEventManager()->m_ON_COLLISION_PRE_SOLVE_Callbacks) {
		onPreSolve(contact, oldManifold);
	}
}

void PhysicsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	for (auto onPostSolve : EventManager::GetEventManager()->m_ON_COLLISION_POST_SOLVE_Callbacks) {
		onPostSolve(contact);
	}
}