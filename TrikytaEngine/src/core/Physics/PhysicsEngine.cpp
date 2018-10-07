#include "PhysicsEngine.h"
#include "PhysicsContactListener.h"

using namespace Physics2D;

/***********************************************/
/********* PHYISCS ENGINE & WORLD PART *********/

PhysicsWorld* PhysicsEngine::_PhysicsWorld = nullptr;



PhysicsWorld* PhysicsEngine::createPhysicsWorld(const PhysicsEngineParams& p_Params) {
	Log("Initing Physics Engine...");
	_PhysicsWorld = new PhysicsWorld(p_Params);
	return _PhysicsWorld;
}

PhysicsWorld* PhysicsEngine::GetPhysicsWorld()
{
	if (_PhysicsWorld == nullptr) {
		Log("You should init physics world before accessing it!")
	}
	return _PhysicsWorld;
}

PhysicsWorld::PhysicsWorld(const PhysicsEngineParams& p_Params) :
	b2World((b2Vec2(p_Params.gravity.x, p_Params.gravity.y))),
	m_VelocityIterations(p_Params.velocityIterations),
	m_PositionIterations(p_Params.positionIterations),
	m_TimeStep(p_Params.timeStep), 
	m_DebugDraw(p_Params.debugDraw)
{
	{
		m_World = this;
		if (m_DebugDraw) {
			m_World->SetDebugDraw(&debugDrawInstance);
			uint32 flags = 0;
			flags += b2Draw::e_shapeBit;
			/*flags += b2Draw::e_aabbBit;
			flags += b2Draw::e_centerOfMassBit;
			flags += b2Draw::e_jointBit;
			flags += b2Draw::e_pairBit;*/
			debugDrawInstance.SetFlags(flags);
		}
		Log("Creating World: %p ", m_World)
	}
}

void PhysicsWorld::update(float dt)
{
	m_World->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
	m_World->DrawDebugData();
}

void PhysicsWorld::AddContactListener()
{
	if (m_PhyContactListener == nullptr) {
		m_World->SetContactListener(PhysicsContactListener::GetPhysicsContactListener());
	}
}

/*************************************/
/********* PHYISCS BODY PART *********/

PhysicsBody* PhysicsBody::CreateBody(PhysicsWorld* p_World, BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions)
{
	BodyDef BodyDef;
	FixtureDef fixtureDef;
	BodyDef.position.Set(p_Position.x, p_Position.y);
	if (p_BodyType == BodyType::STATIC) {
		BodyDef.type = b2_staticBody;
	}
	else if (p_BodyType == BodyType::DYNAMIC) {
		BodyDef.type = b2_dynamicBody;
	}
	b2PolygonShape BodyBox;
	b2ChainShape ChainShape;
	b2CircleShape CircleShape;
	b2PolygonShape PolyShape;
	if (p_Shape == BodyShape::BOX) {
		Vec2f dim = p_Dimentions.at(0);
		BodyBox.SetAsBox(dim.x, dim.y);
		fixtureDef.shape = &BodyBox;
	}
	else if (p_Shape == BodyShape::POLYGON) {
		const int& count = p_Dimentions.size() - 1;
		std::vector<b2Vec2> vertices;
		vertices.reserve(count);
		//TODO CHECK IF CURRENT CODE IS NEEDED IF WE USE b2Vec
		for (auto currentVec : p_Dimentions) {
			if (&currentVec != nullptr) {
				vertices.emplace_back(currentVec.x, currentVec.y);
			}
		}
		b2Vec2* verticesConverted = &vertices[0];
		PolyShape.Set(verticesConverted, count);
		fixtureDef.shape = &PolyShape;
	}
	else if (p_Shape == BodyShape::CHAIN) {
		const int& count = p_Dimentions.size() - 1;
		std::vector<b2Vec2> vertices;
		vertices.reserve(count);
		//TODO CHECK IF CURRENT CODE IS NEEDED IF WE USE b2Vec
		for (auto currentVec : p_Dimentions) {
			if (&currentVec != nullptr) {
				vertices.emplace_back(currentVec.x, currentVec.y);
			}
		}
		b2Vec2* verticesConverted = &vertices[0];
		ChainShape.CreateChain(verticesConverted, count);
		fixtureDef.shape = &ChainShape;
	}
	else if (p_Shape == BodyShape::CIRCLE) {
		Vec2f dim = p_Dimentions.at(0);
		//CircleShape.m_p.Set(dim.x, dim.y); // position
		CircleShape.m_radius = dim.x;
		fixtureDef.shape = &CircleShape;
	}
	fixtureDef.density = p_BodyParams.mass;
	fixtureDef.friction = p_BodyParams.friction;
	fixtureDef.restitution = p_BodyParams.restitution;
	fixtureDef.isSensor = p_BodyParams.isSensor;
	b2Body* Body = p_World->getWorld()->CreateBody(&BodyDef);
	Body->CreateFixture(&fixtureDef);
	return new PhysicsBody(Body, p_BodyType, p_Shape, p_BodyParams);
}