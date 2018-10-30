#include "Joints.h"
#include "core/Physics/PhysicsEngine.h"

using namespace Physics2D;

WheelJointDef::WheelJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor, const b2Vec2& axis)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor);
	localAnchorB = bodyB->GetLocalPoint(anchor);
	localAxisA = bodyA->GetLocalVector(axis);
}

WeldJointDef::WeldJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor);
	localAnchorB = bodyB->GetLocalPoint(anchor);
	referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();
}

RopeJointDef::RopeJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchorA, const b2Vec2& anchorB, float32 max_length)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = anchorA;
	localAnchorB = anchorB;
	maxLength = max_length;
}

RevoluteJointDef::RevoluteJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor);
	localAnchorB = bodyB->GetLocalPoint(anchor);
	referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();
}

PulleyJointDef::PulleyJointDef(PhysicsBody* bA, PhysicsBody* bB,
	const b2Vec2& groundA, const b2Vec2& groundB,
	const b2Vec2& anchorA, const b2Vec2& anchorB,
	float32 r)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	groundAnchorA = groundA;
	groundAnchorB = groundB;
	localAnchorA = bodyA->GetLocalPoint(anchorA);
	localAnchorB = bodyB->GetLocalPoint(anchorB);
	b2Vec2 dA = anchorA - groundA;
	lengthA = dA.Length();
	b2Vec2 dB = anchorB - groundB;
	lengthB = dB.Length();
	ratio = r;
	b2Assert(ratio > b2_epsilon);
}

PrismaticJointDef::PrismaticJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor, const b2Vec2& axis)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor);
	localAnchorB = bodyB->GetLocalPoint(anchor);
	localAxisA = bodyA->GetLocalVector(axis);
	referenceAngle = bodyB->GetAngle() - bodyA->GetAngle();
}

MouseJointDef::MouseJointDef(PhysicsBody* bA, PhysicsBody* bB)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
}

MotorJointDef::MotorJointDef(PhysicsBody* bA, PhysicsBody* bB)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	b2Vec2 xB = bodyB->GetPosition();
	linearOffset = bodyA->GetLocalPoint(xB);

	float32 angleA = bodyA->GetAngle();
	float32 angleB = bodyB->GetAngle();
	angularOffset = angleB - angleA;
}

GearJointDef::GearJointDef(PhysicsBody* bA, PhysicsBody* bB)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
}

FrictionJointDef::FrictionJointDef(PhysicsBody* bA, PhysicsBody* bB, const b2Vec2& anchor)
{
	bodyA = bA->GetBody();
	bodyB = bB->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor);
	localAnchorB = bodyB->GetLocalPoint(anchor);
}

DistanceJointDef::DistanceJointDef(PhysicsBody* b1, PhysicsBody* b2,
	const b2Vec2& anchor1, const b2Vec2& anchor2)
{
	bodyA = b1->GetBody();
	bodyB = b2->GetBody();
	localAnchorA = bodyA->GetLocalPoint(anchor1);
	localAnchorB = bodyB->GetLocalPoint(anchor2);
	b2Vec2 d = anchor2 - anchor1;
	length = d.Length();
}