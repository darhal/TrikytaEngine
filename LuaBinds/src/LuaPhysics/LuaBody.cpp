#include "LStateManager/LStateManager.h"
#include <core/Physics/PhysicsEngine.h>
#include "LuaBody.h"
#include <lua.hpp>

using namespace LuaEngine;

LuaBody* LuaBody::_LUA_BODY_ = nullptr;

void LuaBody::LoadPhysicsBodySystem() 
{
	if (_LUA_BODY_ == nullptr) {
		_LUA_BODY_ = new LuaBody();
	}
	LuaBody::LoadBodyFunction();
}

LuaBody* LuaBody::GetBodyManager() 
{
	return _LUA_BODY_;
}

void LuaBody::LoadBodyFunction()
{
	auto L = LStateManager::GetLuaState();

	lua_pushcfunction(L, LuaBody::CreateBody);
	lua_setglobal(L, "createBody");

	lua_pushcfunction(L, LuaBody::SetLinearVelocity);
	lua_setglobal(L, "setLinearVelocity");
}

int LuaBody::CreateBody(lua_State *L)
{
	const char* bodyType = lua_tostring(L, 1);
	const char* bodyShape = lua_tostring(L, 2);
	float mass = (float)lua_tonumber(L, 3);
	float friction = (float)lua_tonumber(L, 4);
	float px = (float)lua_tonumber(L, 5);
	float py = (float)lua_tonumber(L, 6);
	float sx = (float)lua_tonumber(L, 7);
	float sy = (float)lua_tonumber(L, 8);

	Physics2D::BodyShape rBodyShape;
	Physics2D::BodyType rBodyType;
	if (bodyType == "Dynamic") {
		rBodyType = Physics2D::BodyType::DYNAMIC;
	}else if (bodyType == "Static") {
		rBodyType = Physics2D::BodyType::STATIC;
	}else if (bodyType == "Kinematic") {
		rBodyType = Physics2D::BodyType::KINEMATIC;
	}
	if (bodyShape == "Box") {
		rBodyShape = Physics2D::BodyShape::BOX;
	}else if (bodyShape == "Circle") {
		rBodyShape = Physics2D::BodyShape::CIRCLE;
	}

	auto body = Physics2D::PhysicsBody::CreateBody
	(
		Physics2D::PhysicsEngine::GetPhysicsWorld(), rBodyType,
		rBodyShape, Physics2D::BodyParams{mass, friction},
		Vec2f{px, py},
		std::vector<Vec2f>{Vec2f(sx, sy)}
	);

	lua_pushlightuserdata(L, (void*)body);
	return 1;
}

int LuaBody::SetLinearVelocity(lua_State *L)
{
	Physics2D::PhysicsBody* bodyType = (Physics2D::PhysicsBody*)lua_touserdata(L, 1);
	float vx = (float)lua_tonumber(L, 2);
	float vy = (float)lua_tonumber(L, 3);
	bodyType->SetLinearVelocity(Vec2f(vx, vy));

	lua_pushboolean(L, true);
	return 1;
}

int LuaBody::SetAngularDamping(lua_State* L)
{
	Physics2D::PhysicsBody* bodyType = (Physics2D::PhysicsBody*)lua_touserdata(L, 1);
	float r = (float)lua_tonumber(L, 2);
	bodyType->SetAngularDamping(r);

	lua_pushboolean(L, true);
	return 1;
}