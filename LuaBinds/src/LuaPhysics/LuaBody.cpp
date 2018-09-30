#include "LStateManager/LStateManager.h"
#include <core/Physics/PhysicsEngine.h>
#include "LuaBody.h"
#include <lua.hpp>
#include "LuaCore/ErrorManager.h"

using namespace LuaEngine;
using namespace Physics2D;

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

	lua_pushcfunction(L, LuaBody::CreateBodyV2);
	lua_setglobal(L, "createBodyV2");

	lua_pushcfunction(L, LuaBody::SetLinearVelocity);
	lua_setglobal(L, "setLinearVelocity");

	lua_pushcfunction(L, LuaBody::SetAngularDamping);
	lua_setglobal(L, "setAngularDamping");

	lua_pushcfunction(L, LuaBody::GetLinearVelocity);
	lua_setglobal(L, "getLinearVelocity");

	lua_pushcfunction(L, LuaBody::GetAngularDamping);
	lua_setglobal(L, "getAngularDamping");

	lua_pushcfunction(L, LuaBody::GetBodyPosition);
	lua_setglobal(L, "getBodyPosition");

	lua_pushcfunction(L, LuaBody::SetBodyTransform);
	lua_setglobal(L, "setBodyTransform");
}

int LuaBody::GetLinearVelocity(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	PhysicsBody* body = (PhysicsBody*)lua_touserdata(L, 1);
	lua_pushnumber(L, body->GetLinearVelocity().x);
	lua_pushnumber(L, body->GetLinearVelocity().y);
	return 2;
}

int LuaBody::GetAngularDamping(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	PhysicsBody* body = (PhysicsBody*)lua_touserdata(L, 1);
	lua_pushnumber(L, body->GetAngularDamping());
	return 1;
}

int LuaBody::GetBodyPosition(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	PhysicsBody* body = (PhysicsBody*)lua_touserdata(L, 1);
	lua_pushnumber(L, body->GetPosition().x);
	lua_pushnumber(L, body->GetPosition().y);
	return 2;
}

int LuaBody::SetBodyTransform(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnnn")) {
		return 1;
	}
	PhysicsBody* body = (PhysicsBody*)lua_touserdata(L, 1);
	body->SetTransform(Vec2f((float)lua_tonumber(L,2), (float)lua_tonumber(L, 3)), (float)lua_tonumber(L, 4));
	return 0;
}


int LuaBody::CreateBodyV2(lua_State *L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "ttnnt")) {
		return 1;
	}

	lua_getfield(L, 1, "type");
	const char* bodyType = lua_tostring(L, -1);
	lua_getfield(L, 1, "shape");
	const char* bodyShape = lua_tostring(L, -1);

	lua_getfield(L, 2, "mass");
	float mass = (float)lua_tonumber(L, -1);
	lua_getfield(L, 2, "friction");
	float friction = (float)lua_tonumber(L, -1);
	lua_getfield(L, 2, "restitution");
	float restitution = (float)lua_tonumber(L, -1);
	lua_getfield(L, 2, "sensor");
	bool sensor = (bool)lua_toboolean(L, -1);

	float px = (float)lua_tonumber(L, 7);
	float py = (float)lua_tonumber(L, 8);
	//lua_type(L, 1);
	std::vector<Vec2f> coords;
	Vec2f tempV;
	int n = lua_rawlen(L, 9);
	if (n == 1) {
		LogConsole(MESSAGE_TYPE::ERROR, "Expected a table with length >= 2 at argument 2");
		return 0;
	}
	for (int i = 1; i <= n; i++) {
		lua_rawgeti(L, 9, i);  
		if (i % 2 != 0)
		{
			tempV.x = (float)lua_tonumber(L, -1);
		}else if (i % 2 == 0){
			tempV.y = (float)lua_tonumber(L, -1);
			coords.emplace_back(tempV.x, tempV.y);
		}
	}

	Physics2D::BodyShape rBodyShape;
	Physics2D::BodyType rBodyType;
	if (strcmp(bodyType,"Dynamic") ==0) {
		rBodyType = Physics2D::BodyType::DYNAMIC;
	}else if (strcmp(bodyType, "Static") == 0) {
		rBodyType = Physics2D::BodyType::STATIC;
	}else if (strcmp(bodyType, "Kinematic") == 0) {
		rBodyType = Physics2D::BodyType::KINEMATIC;
	}
	if (strcmp(bodyShape, "Box") == 0) {
		rBodyShape = Physics2D::BodyShape::BOX;
	}else if (strcmp(bodyShape, "Circle") == 0) {
		rBodyShape = Physics2D::BodyShape::CIRCLE;
	}else if (strcmp(bodyShape, "Polygon") == 0) {
		rBodyShape = Physics2D::BodyShape::POLYGON;
	}

	auto body = Physics2D::PhysicsBody::CreateBody
	(
		Physics2D::PhysicsEngine::GetPhysicsWorld(), rBodyType,
		rBodyShape, Physics2D::BodyParams{mass, friction, restitution, sensor},
		Vec2f{px, py}, coords
		//std::vector<Vec2f>{Vec2f(sx, sy)}
	);

	lua_pushlightuserdata(L, (void*)body);
	return 1;
}

int LuaBody::CreateBody(lua_State *L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "ssnnnbnnt")) {
		return 1;
	}
	const char* bodyType = lua_tostring(L, 1);
	const char* bodyShape = lua_tostring(L, 2);
	float mass = (float)lua_tonumber(L, 3);
	float friction = (float)lua_tonumber(L, 4);
	float res = (float)lua_tonumber(L, 5);
	bool sensor = (bool)lua_toboolean(L, 6);
	float px = (float)lua_tonumber(L, 7);
	float py = (float)lua_tonumber(L, 8);
	//lua_type(L, 1);
	std::vector<Vec2f> coords;
	Vec2f tempV;
	int n = lua_rawlen(L, 9);
	if (n == 1) {
		LogConsole(MESSAGE_TYPE::ERROR, "Expected a table with length >= 2 at argument 2");
		return 0;
	}
	for (int i = 1; i <= n; i++) {
		lua_rawgeti(L, 9, i);
		if (i % 2 != 0)
		{
			tempV.x = (float)lua_tonumber(L, -1);
		}
		else if (i % 2 == 0) {
			tempV.y = (float)lua_tonumber(L, -1);
			coords.emplace_back(tempV.x, tempV.y);
		}
	}

	Physics2D::BodyShape rBodyShape;
	Physics2D::BodyType rBodyType;
	if (strcmp(bodyType, "Dynamic") == 0) {
		rBodyType = Physics2D::BodyType::DYNAMIC;
	}
	else if (strcmp(bodyType, "Static") == 0) {
		rBodyType = Physics2D::BodyType::STATIC;
	}
	else if (strcmp(bodyType, "Kinematic") == 0) {
		rBodyType = Physics2D::BodyType::KINEMATIC;
	}
	if (strcmp(bodyShape, "Box") == 0) {
		rBodyShape = Physics2D::BodyShape::BOX;
	}
	else if (strcmp(bodyShape, "Circle") == 0) {
		rBodyShape = Physics2D::BodyShape::CIRCLE;
	}
	else if (strcmp(bodyShape, "Polygon") == 0) {
		rBodyShape = Physics2D::BodyShape::POLYGON;
	}

	auto body = Physics2D::PhysicsBody::CreateBody
	(
		Physics2D::PhysicsEngine::GetPhysicsWorld(), rBodyType,
		rBodyShape, Physics2D::BodyParams{ mass, friction, res, sensor },
		Vec2f{ px, py }, coords
		//std::vector<Vec2f>{Vec2f(sx, sy)}
	);

	lua_pushlightuserdata(L, (void*)body);
	return 1;
}

int LuaBody::SetLinearVelocity(lua_State *L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnn")) {
		return 1;
	}
	Physics2D::PhysicsBody* bodyType = (Physics2D::PhysicsBody*)lua_touserdata(L, 1);
	float vx = (float)lua_tonumber(L, 2);
	float vy = (float)lua_tonumber(L, 3);
	bodyType->SetLinearVelocity(Vec2f(vx, vy));

	lua_pushboolean(L, true);
	return 1;
}

int LuaBody::SetAngularDamping(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "ln")) {
		return 1;
	}
	Physics2D::PhysicsBody* bodyType = (Physics2D::PhysicsBody*)lua_touserdata(L, 1);
	float r = (float)lua_tonumber(L, 2);
	bodyType->SetAngularDamping(r);

	lua_pushboolean(L, true);
	return 1;
}


