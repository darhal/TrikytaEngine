#pragma once
#include <PhysicsEngine/Core/Box2D.h>
#include <core/Common/defines.h>

namespace Physics2D {
	class WheelJointDef : public b2WheelJointDef {
		WheelJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor, const b2Vec2& axis);
	};

	class WeldJointDef : public b2WeldJointDef {
		WeldJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor);
	};

	class RopeJointDef : public b2RopeJointDef {
		RopeJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchorA=b2Vec2(-1.0f, 0.0f), const b2Vec2& anchorB =b2Vec2(1.0f, 0.0f), float32 max_length=0.0f);
	};

	class RevoluteJointDef : public b2RevoluteJointDef {
		RevoluteJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor);
	};

	class PulleyJointDef : public b2PulleyJointDef {
		PulleyJointDef(PhysicsBody* bodyA, PhysicsBody* bodyB,
			const b2Vec2& groundAnchorA, const b2Vec2& groundAnchorB,
			const b2Vec2& anchorA, const b2Vec2& anchorB,
			float32 ratio);
	};

	class PrismaticJointDef : public b2PrismaticJointDef {
		PrismaticJointDef(PhysicsBody* bodyA, PhysicsBody* bodyB, const b2Vec2& anchor, const b2Vec2& axis);
	};

	class MouseJointDef : public b2MouseJointDef {
		MouseJointDef(PhysicsBody* bodyA, PhysicsBody* bodyB);
	};

	class MotorJointDef : public b2MotorJointDef {
		MotorJointDef(PhysicsBody* bA, PhysicsBody* bB);
	};

	class GearJointDef : public b2GearJointDef {
		GearJointDef(PhysicsBody* bA, PhysicsBody* bB);
	};

	class FrictionJointDef : public b2FrictionJointDef{
		FrictionJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor);
	};

	class DistanceJointDef : public b2DistanceJointDef {
		DistanceJointDef(PhysicsBody* b1, PhysicsBody* b2,
			const b2Vec2& anchor1, const b2Vec2& anchor2);
	};
}