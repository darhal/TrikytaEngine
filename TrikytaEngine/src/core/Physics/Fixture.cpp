#include "Fixture.h"
#include "core/Physics/PhysicsEngine.h"

using namespace Physics2D;

FixtureDef::FixtureDef(BodyType p_BodyType, BodyShape p_Shape, BodyParams p_BodyParams, Vec2f p_Position, std::vector<Vec2f> p_Dimentions) 
{
	b2PolygonShape BodyBox;
	b2ChainShape ChainShape;
	b2CircleShape CircleShape;
	b2PolygonShape PolyShape;
	if (p_Shape == BodyShape::BOX) {
		Vec2f dim = p_Dimentions.at(0);
		BodyBox.SetAsBox(dim.x, dim.y);
		shape = &BodyBox;
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
		PolyShape.m_centroid.Set(p_Position.x, p_Position.y);
		shape = &PolyShape;
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
		shape = &ChainShape;
	}
	else if (p_Shape == BodyShape::CIRCLE) {
		Vec2f dim = p_Dimentions.at(0);
		CircleShape.m_p.Set(p_Position.x, p_Position.y); // position
		CircleShape.m_radius = dim.x;
		shape = &CircleShape;
	}
	density = p_BodyParams.mass;
	friction = p_BodyParams.friction;
	restitution = p_BodyParams.restitution;
	isSensor = p_BodyParams.isSensor;
}