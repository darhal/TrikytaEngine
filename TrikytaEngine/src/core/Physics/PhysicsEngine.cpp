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
	this->AddContactListener();
}

void PhysicsWorld::DestroyBody(PhysicsBody* body) {
	b2World::DestroyBody(body->GetBody()); // this will crash physics engine!
	FREE(body);
}

void PhysicsWorld::QueueDelete(b2Body* body) {
	m_DeleteQueue.emplace_back(body);
}

void PhysicsWorld::update(float /*dt*/)
{
	m_World->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
	PhysicsWorld::ClearQueue(); // Clear trash!
	if (m_DebugDraw) 
		m_World->DrawDebugData();
}

void PhysicsWorld::ClearQueue()
{
	for (auto& body : m_DeleteQueue) {
		b2World::DestroyBody(body);
	}
	m_DeleteQueue.clear();
	for (auto& bodyToCreate : m_CreateQueue) {
		CreateBodyInQueue(bodyToCreate);
	}
	m_CreateQueue.clear();
}

void PhysicsWorld::CreateBodyInQueue(const PhysicsParamsWarpper& bodyToCreate)
{
	b2Body* Body = PhysicsEngine::CreateBodyHelper(this, bodyToCreate.m_BodyType, bodyToCreate.m_Shape, bodyToCreate.m_BodyParams, bodyToCreate.m_Position, bodyToCreate.m_Dimentions);
	bodyToCreate.m_TempBody->m_BodyType = bodyToCreate.m_BodyType;
	bodyToCreate.m_TempBody->m_Shape = bodyToCreate.m_Shape;
	bodyToCreate.m_TempBody->m_BodyParams = bodyToCreate.m_BodyParams;
	bodyToCreate.m_TempBody->setBody(Body);
}

void PhysicsWorld::QueueCreate(PhysicsBody* temp_body, BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions)
{
	// Anticpate of the same body is the delete queue ! (Not sure if it gonna work proerly bcuz of PhysicsBody and b2Body and probably will never happen!)
	/*if (std::find(m_DeleteQueue.begin(), m_DeleteQueue.end(), temp_body->GetBody()) != m_DeleteQueue.end()) {
		FREE(temp_body);
		return;
	}*/
	m_CreateQueue.emplace_back(p_BodyType, p_Shape, p_BodyParams, p_Position, p_Dimentions, temp_body);
}

void PhysicsWorld::AddContactListener()
{
	if (m_PhyContactListener == nullptr) {
		m_World->SetContactListener(PhysicsContactListener::GetPhysicsContactListener());
	}
}

void PhysicsWorld::deleteDebugger()
{
	m_DebugDraw = false;
}

void PhysicsWorld::setDebugger()
{
	m_DebugDraw = true;
	if (getDebugDraw() == nullptr) {
		m_World->SetDebugDraw(&debugDrawInstance);
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		/*flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_centerOfMassBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_pairBit;*/
		debugDrawInstance.SetFlags(flags);
	}
}

/*************************************/
/********* PHYISCS BODY PART *********/

PhysicsBody* PhysicsBody::CreateBody(PhysicsWorld* p_World,const BodyType& p_BodyType,const BodyShape& p_Shape,const BodyParams& p_BodyParams,const Vec2f& p_Position,const std::vector<Vec2f>& p_Dimentions)
{
	if (p_World == nullptr) { return NULL; } // we have no choice!
	if (p_World->IsLocked()) {
		auto temp_body =  new PhysicsBody();
		p_World->QueueCreate(temp_body, p_BodyType, p_Shape, p_BodyParams, p_Position, p_Dimentions);
		return temp_body; // musnt return a NULL ! this is just a temp body !
	}
	b2Body* Body =  PhysicsEngine::CreateBodyHelper(p_World, p_BodyType, p_Shape, p_BodyParams, p_Position, p_Dimentions);
	return new PhysicsBody(Body, p_BodyType, p_Shape, p_BodyParams);
}

b2Body* PhysicsEngine::CreateBodyHelper(PhysicsWorld* p_World, BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions)
{
	BodyDef BodyDef;
	b2FixtureDef fixtureDef;
	BodyDef.position.Set(p_Position.x, p_Position.y);
	if (p_BodyType == BodyType::STATIC) {
		BodyDef.type = b2_staticBody;
	}
	else if (p_BodyType == BodyType::DYNAMIC) {
		BodyDef.type = b2_dynamicBody;
	}
	else if (p_BodyType == BodyType::KINEMATIC) {
		BodyDef.type = b2_kinematicBody;
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
	fixtureDef.isSensor = p_BodyParams.sensor;
	b2Body* out_body = p_World->getWorld()->CreateBody(&BodyDef);
	out_body->CreateFixture(&fixtureDef);
	return out_body;
}