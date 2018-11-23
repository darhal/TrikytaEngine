#pragma once
#include <vector>
#include "core/Objects/Object.h"
#include <PhysicsEngine/Core/Box2D.h>
#include <PhysicsEngine/DebugDraw/b2GLDraw.h>
#include "core/Common/defines.h"
#include "core/Common/Vec2.h"
#include "core/Physics/Fixture.h"

class PhysicsContactListener;

namespace Physics2D {
	typedef b2Fixture Fixture;
	typedef b2Transform Transform;
	typedef b2ContactEdge ContactEdge;
	typedef b2JointEdge JointEdge;
	typedef b2Shape Shape;
	//typedef b2FixtureDef FixtureDef;
	typedef b2BodyDef BodyDef;

	enum class BodyType {STATIC, DYNAMIC, KINEMATIC};
	enum class BodyShape {BOX, CIRCLE, POLYGON, CHAIN, EDGE};

	// Struct contain Physics Propreties {float mass,float friction,float restitution, bool isSensor}
	struct BodyParams 
	{ 
		BodyParams(float pmass = 1.0, float pfriction = 0.2, float prestitution = 0.0f, bool pisSensor = false)
			: mass(pmass), friction(pfriction), restitution(prestitution), sensor(pisSensor)
		{}
		float mass = 1.0; float friction = 1.0; float restitution = 0.0f; bool sensor = false;
	};

	struct PhysicsEngineParams 
	{ 
		PhysicsEngineParams(Vec2f pgravity={ 0, 10.f }, float ptimeStep = 1 / 60.f, int pvelocityIterations = 8, int ppositionIterations = 3, bool pdebugDraw = true)
			: gravity(pgravity), timeStep(ptimeStep), velocityIterations(pvelocityIterations), positionIterations(ppositionIterations), debugDraw(pdebugDraw)
		{}
		Vec2f gravity{ 0, 10.f }; float timeStep = 1 / 60.f; int velocityIterations = 8; int positionIterations = 3; bool debugDraw = true; 
	};

	class PhysicsWorld;

	class PhysicsEngine {
	public:
		static PhysicsWorld* createPhysicsWorld(const PhysicsEngineParams& p_Params);
		static PhysicsWorld* GetPhysicsWorld();
		static PhysicsWorld* _PhysicsWorld;
		friend class PhysicsBody;
		friend class PhysicsWorld;
	private:
		static b2Body* CreateBodyHelper(PhysicsWorld* p_World, BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions);
	};

	class PhysicsWorld : public PhysicsEngine, public b2World {
	protected:
		PhysicsWorld(const PhysicsEngineParams&);
	public:
		void AddContactListener();
		inline b2World* getWorld() {return m_World;}
		void update(float);

		void deleteDebugger();
		void setDebugger();
		void DestroyBody(PhysicsBody* body);
		void ClearQueue();
		friend PhysicsEngine;
		friend class PhysicsBody;
	private:
		struct PhysicsParamsWarpper {
			PhysicsParamsWarpper(BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions, PhysicsBody* temp_body):
				m_BodyType(p_BodyType),  m_Shape(p_Shape), m_BodyParams(p_BodyParams), m_Position(p_Position), m_Dimentions(p_Dimentions), m_TempBody(temp_body)
			{}
			BodyType m_BodyType; BodyShape m_Shape; BodyParams m_BodyParams; Vec2f m_Position; std::vector<Vec2f> m_Dimentions; PhysicsBody* m_TempBody;
		};
		b2GLDraw debugDrawInstance;
		b2World* m_World;
		bool m_DebugDraw;
		float m_TimeStep;
		int m_VelocityIterations, m_PositionIterations;
		class ::PhysicsContactListener* m_PhyContactListener = nullptr;
		std::vector<b2Body*> m_DeleteQueue;
		std::vector<PhysicsParamsWarpper> m_CreateQueue;
		void QueueDelete(b2Body* body);
		void QueueCreate(PhysicsBody*, BodyType, BodyShape, BodyParams, Vec2f, std::vector<Vec2f>);
		void CreateBodyInQueue(const PhysicsParamsWarpper&);
	};

	class PhysicsBody : public Object {
	protected:
		PhysicsBody(b2Body* p_Body, BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams) :
			m_Body(p_Body), m_BodyType(p_BodyType), m_Shape(p_Shape), m_BodyParams(p_BodyParams)
		{
			m_Body->m_physicsBody = this;
		}
		PhysicsBody() {}
	public:
		
		static PhysicsBody* CreateBody
		(
			PhysicsWorld* p_World, BodyType p_BodyType,
			BodyShape p_Shape, BodyParams p_BodyParams,
			Vec2f p_Position, std::vector<Vec2f> p_Dimentions
		);
		~PhysicsBody() 
		{
			//m_Body->GetWorld()->DestroyBody(m_Body); // this will crash physics engine!
			if (PhysicsEngine::GetPhysicsWorld() != NULL) {
				PhysicsEngine::GetPhysicsWorld()->QueueDelete(m_Body);
			}
		};
		b2Body* GetBody() const { return m_Body; }
		virtual Vec2i getPosition() override { return Vec2i((int)m_Body->GetPosition().x, (int)m_Body->GetPosition().y); }
		virtual double GetRotation() override { return Utility::ToDegree(GetAngle()); }
	private:
		inline void setBody(b2Body* body) { m_Body = body; m_Body->m_physicsBody = this; }
		friend class b2Body;
		friend class PhysicsWorld;
		b2Body* m_Body;
		BodyType m_BodyType;
		BodyShape m_Shape;
		BodyParams m_BodyParams;
	public:
		/// Creates a fixture and attach it to this body. Use this function if you need
		/// to set some fixture parameters, like friction. Otherwise you can create the
		/// fixture directly from a shape.
		/// If the density is non-zero, this function automatically updates the mass of the body.
		/// Contacts are not created until the next time step.
		/// @param def the fixture definition.
		/// @warning This function is locked during callbacks.
		Fixture* CreateFixture(const FixtureDef* def) 
		{ 
			/*b2FixtureDef fix;
			fix.shape = def->shape;
			fix.density = def->density;
			fix.friction = def->friction;
			fix.isSensor = def->isSensor;
			fix.restitution = def->restitution;
			fix.filter = def->filter;
			return m_Body->CreateFixture(&fix);*/
			return m_Body->CreateFixture(def);
		}

		/// Creates a fixture from a shape and attach it to this body.
		/// This is a convenience function. Use b2FixtureDef if you need to set parameters
		/// like friction, restitution, user data, or filtering.
		/// If the density is non-zero, this function automatically updates the mass of the body.
		/// @param shape the shape to be cloned.
		/// @param density the shape density (set to zero for static bodies).
		/// @warning This function is locked during callbacks.
		Fixture* CreateFixture(const Shape* shape, float32 density) { return m_Body->CreateFixture(shape, density); }

		/// Destroy a fixture. This removes the fixture from the broad-phase and
		/// destroys all contacts associated with this fixture. This will
		/// automatically adjust the mass of the body if the body is dynamic and the
		/// fixture has positive density.
		/// All fixtures attached to a body are implicitly destroyed when the body is destroyed.
		/// @param fixture the fixture to be removed.
		/// @warning This function is locked during callbacks.
		void DestroyFixture(Fixture* fixture) { m_Body->DestroyFixture(fixture); }

		/// Set the linear velocity of the center of mass.
		/// @param v the new linear velocity of the center of mass.
		inline void SetLinearVelocity(Vec2f p_Force) { m_Body->SetLinearVelocity(p_Force); }

		/// Set the position of the body's origin and rotation.
		/// Manipulating a body's transform may cause non-physical behavior.
		/// Note: contacts are updated on the next call to b2World::Step.
		/// @param position the world position of the body's local origin.
		/// @param angle the world rotation in radians.
		void SetTransform(const Vec2f& p_Position, float32 p_Angle) { m_Body->SetTransform(p_Position, p_Angle); };

		/// Get the body transform for the body's origin.
		/// @return the world transform of the body's origin.
		const Transform& GetTransform() const { return m_Body->GetTransform(); };

		/// Get the world body origin position.
		/// @return the world position of the body's origin.
		const Vec2f& GetPosition() const { return m_Body->GetPosition(); };

		/// Get the angle in radians.
		/// @return the current world rotation angle in radians.
		float32 GetAngle() const { return m_Body->GetAngle(); };

		/// Get the world position of the center of mass.
		const Vec2f& GetWorldCenter() const { return m_Body->GetWorldCenter(); };

		/// Get the local position of the center of mass.
		const Vec2f& GetLocalCenter() const { return m_Body->GetLocalCenter(); }

		/// Get the linear velocity of the center of mass.
		/// @return the linear velocity of the center of mass.
		const Vec2f& GetLinearVelocity() const {return m_Body->GetLinearVelocity();}

		/// Set the angular velocity.
		/// @param omega the new angular velocity in radians/second.
		void SetAngularVelocity(float32 omega) { m_Body->SetAngularVelocity(omega); }

		/// Get the angular velocity.
		/// @return the angular velocity in radians/second.
		float32 GetAngularVelocity() const { return m_Body->GetAngularVelocity(); };

		/// Apply a force at a world point. If the force is not
		/// applied at the center of mass, it will generate a torque and
		/// affect the angular velocity. This wakes up the body.
		/// @param force the world force vector, usually in Newtons (N).
		/// @param point the world position of the point of application.
		/// @param wake also wake up the body
		void ApplyForce(const Vec2f& force, const Vec2f& point, bool wake) { m_Body->ApplyForce(force, point, wake); }

		/// Apply a force to the center of mass. This wakes up the body.
		/// @param force the world force vector, usually in Newtons (N).
		/// @param wake also wake up the body
		void ApplyForceToCenter(const Vec2f& force, bool wake) { m_Body->ApplyForceToCenter(force, wake); }

		/// Apply a torque. This affects the angular velocity
		/// without affecting the linear velocity of the center of mass.
		/// @param torque about the z-axis (out of the screen), usually in N-m.
		/// @param wake also wake up the body
		void ApplyTorque(float32 torque, bool wake) { m_Body->ApplyTorque(torque, wake); }

		/// Apply an impulse at a point. This immediately modifies the velocity.
		/// It also modifies the angular velocity if the point of application
		/// is not at the center of mass. This wakes up the body.
		/// @param impulse the world impulse vector, usually in N-seconds or kg-m/s.
		/// @param point the world position of the point of application.
		/// @param wake also wake up the body
		void ApplyLinearImpulse(const Vec2f& impulse, const Vec2f& point, bool wake) { m_Body->ApplyLinearImpulse(impulse, point, wake); }

		/// Apply an impulse to the center of mass. This immediately modifies the velocity.
		/// @param impulse the world impulse vector, usually in N-seconds or kg-m/s.
		/// @param wake also wake up the body
		void ApplyLinearImpulseToCenter(const Vec2f& impulse, bool wake) { m_Body->ApplyLinearImpulseToCenter(impulse, wake); }

		/// Apply an angular impulse.
		/// @param impulse the angular impulse in units of kg*m*m/s
		/// @param wake also wake up the body
		void ApplyAngularImpulse(float32 impulse, bool wake) { m_Body->ApplyAngularImpulse(impulse, wake); }

		/// Get the total mass of the body.
		/// @return the mass, usually in kilograms (kg).
		float32 GetMass() const { return m_Body->GetMass(); }

		/// Get the rotational inertia of the body about the local origin.
		/// @return the rotational inertia, usually in kg-m^2.
		float32 GetInertia() const { return m_Body->GetInertia(); }

		/// Get the mass data of the body.
		/// @return a struct containing the mass, inertia and center of the body.
		void GetMassData(b2MassData* data) const { m_Body->GetMassData(data); }

		/// Set the mass properties to override the mass properties of the fixtures.
		/// Note that this changes the center of mass position.
		/// Note that creating or destroying fixtures can also alter the mass.
		/// This function has no effect if the body isn't dynamic.
		/// @param massData the mass properties.
		void SetMassData(const b2MassData* data) { m_Body->SetMassData(data); }

		/// This resets the mass properties to the sum of the mass properties of the fixtures.
		/// This normally does not need to be called unless you called SetMassData to override
		/// the mass and you later want to reset the mass.
		void ResetMassData() { m_Body->ResetMassData(); }

		/// Get the world coordinates of a point given the local coordinates.
		/// @param localPoint a point on the body measured relative the the body's origin.
		/// @return the same point expressed in world coordinates.
		Vec2f GetWorldPoint(const Vec2f& localPoint) const { return m_Body->GetWorldPoint(localPoint); }

		/// Get the world coordinates of a vector given the local coordinates.
		/// @param localVector a vector fixed in the body.
		/// @return the same vector expressed in world coordinates.
		Vec2f GetWorldVector(const Vec2f& localVector) const { return m_Body->GetWorldVector(localVector); }

		/// Gets a local point relative to the body's origin given a world point.
		/// @param a point in world coordinates.
		/// @return the corresponding local point relative to the body's origin.
		Vec2f GetLocalPoint(const Vec2f& worldPoint) const { return m_Body->GetLocalPoint(worldPoint); }

		/// Gets a local vector given a world vector.
		/// @param a vector in world coordinates.
		/// @return the corresponding local vector.
		Vec2f GetLocalVector(const Vec2f& worldVector) const { return m_Body->GetLocalVector(worldVector); }

		/// Get the world linear velocity of a world point attached to this body.
		/// @param a point in world coordinates.
		/// @return the world velocity of a point.
		Vec2f GetLinearVelocityFromWorldPoint(const Vec2f& worldPoint) const { return m_Body->GetLinearVelocityFromWorldPoint(worldPoint); }

		/// Get the world velocity of a local point.
		/// @param a point in local coordinates.
		/// @return the world velocity of a point.
		Vec2f GetLinearVelocityFromLocalPoint(const Vec2f& localPoint) const { return m_Body->GetLinearVelocityFromLocalPoint(localPoint); }

		/// Get the linear damping of the body.
		float32 GetLinearDamping() const { return m_Body->GetLinearDamping(); }

		/// Set the linear damping of the body.
		void SetLinearDamping(float32 linearDamping) { m_Body->SetLinearDamping(linearDamping); }

		/// Get the angular damping of the body.
		float32 GetAngularDamping() const { return m_Body->GetAngularDamping(); }

		/// Set the angular damping of the body.
		void SetAngularDamping(float32 angularDamping) { m_Body->SetAngularDamping(angularDamping); }

		/// Get the gravity scale of the body.
		float32 GetGravityScale() const { return m_Body->GetGravityScale(); }

		/// Set the gravity scale of the body.
		void SetGravityScale(float32 scale) { m_Body->SetGravityScale(scale); }

		/// Set the type of this body. This may alter the mass and velocity.
		void SetType(BodyType type) {
			if (type == BodyType::DYNAMIC)
				m_Body->SetType(b2_dynamicBody);
			else if (type == BodyType::STATIC)
				m_Body->SetType(b2_staticBody);
			else if (type == BodyType::KINEMATIC)
				m_Body->SetType(b2_kinematicBody);
		}

		/// Get the type of this body.
		BodyType GetType() const {
			if (m_Body->GetType() == b2_dynamicBody)
				return BodyType::DYNAMIC;
			else if (m_Body->GetType() == b2_staticBody)
				return BodyType::STATIC;
			else if (m_Body->GetType() == b2_kinematicBody)
				return BodyType::KINEMATIC;
		}

		/// Should this body be treated like a bullet for continuous collision detection?
		void SetBullet(bool flag) {
			m_Body->SetBullet(flag);
		}

		/// Is this body treated like a bullet for continuous collision detection?
		bool IsBullet() const { return m_Body->IsBullet(); }

		/// You can disable sleeping on this body. If you disable sleeping, the
		/// body will be woken.
		void SetSleepingAllowed(bool flag) { m_Body->SetSleepingAllowed(flag); }

		/// Is this body allowed to sleep
		bool IsSleepingAllowed() const { return m_Body->IsSleepingAllowed(); }

		/// Set the sleep state of the body. A sleeping body has very
		/// low CPU cost.
		/// @param flag set to true to wake the body, false to put it to sleep.
		void SetAwake(bool flag) { m_Body->SetAwake(flag); }

		/// Get the sleeping state of this body.
		/// @return true if the body is awake.
		bool IsAwake() const { return m_Body->IsAwake(); }

		/// Set the active state of the body. An inactive body is not
		/// simulated and cannot be collided with or woken up.
		/// If you pass a flag of true, all fixtures will be added to the
		/// broad-phase.
		/// If you pass a flag of false, all fixtures will be removed from
		/// the broad-phase and all contacts will be destroyed.
		/// Fixtures and joints are otherwise unaffected. You may continue
		/// to create/destroy fixtures and joints on inactive bodies.
		/// Fixtures on an inactive body are implicitly inactive and will
		/// not participate in collisions, ray-casts, or queries.
		/// Joints connected to an inactive body are implicitly inactive.
		/// An inactive body is still owned by a b2World object and remains
		/// in the body list.
		void SetActive(bool flag) { return m_Body->SetActive(flag); }

		/// Get the active state of the body.
		bool IsActive() const { return m_Body->IsActive(); }

		/// Set this body to have fixed rotation. This causes the mass
		/// to be reset.
		void SetFixedRotation(bool flag) { m_Body->SetFixedRotation(flag); }

		/// Does this body have fixed rotation?
		bool IsFixedRotation() const { return m_Body->IsFixedRotation(); }

		/// Get the list of all fixtures attached to this body.
		Fixture* GetFixtureList() { return m_Body->GetFixtureList(); }
		const Fixture* GetFixtureList() const { return m_Body->GetFixtureList(); }

		/// Get the list of all joints attached to this body.
		JointEdge* GetJointList() { m_Body->GetJointList(); }
		const JointEdge* GetJointList() const { return m_Body->GetJointList(); }

		/// Get the list of all contacts attached to this body.
		/// @warning this list changes during the time step and you may
		/// miss some collisions if you don't use b2ContactListener.
		ContactEdge* GetContactList() { return m_Body->GetContactList(); }
		const ContactEdge* GetContactList() const { return m_Body->GetContactList(); }

		/// Get the next body in the world's body list.
		b2Body* GetNext() { return m_Body->GetNext(); }
		const b2Body* GetNext() const { return m_Body->GetNext(); }

		/// Get the user data pointer that was provided in the body definition.
		void* GetUserData() const { return m_Body->GetUserData(); }

		/// Set the user data. Use this to store your application specific data.
		void SetUserData(void* data) { m_Body->SetUserData(data); }
	};
}