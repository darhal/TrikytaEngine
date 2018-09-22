#pragma once
#include <PhysicsEngine/Core/Box2D.h>

class PhysicsContactListener : public b2ContactListener
{
public:
	static PhysicsContactListener* GetPhysicsContactListener();

	virtual void BeginContact(b2Contact*);
	virtual void EndContact(b2Contact*);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	friend class EventManager;
private:
	static PhysicsContactListener* _PhysicsContactListener;
};
