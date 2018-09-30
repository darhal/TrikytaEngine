#include <lua.hpp>
#include "LuaBridge/LuaBridge.h"
#include "LuaSprite.h"
#include <core/Drawable/Sprite.h>
#include "LStateManager/LStateManager.h"
#include <core/Common/Vec2.h>
#include <core/Objects/ObjectHandler.h>
#include <core/Physics/PhysicsEngine.h>
#include "Shared/ldefines.h"
#include <core/Drawable/Animation.h>
#include <UI/UIText.h>
#include "LuaCore/ErrorManager.h"
#include <iostream>


using namespace LuaEngine;

LuaSprite* LuaSprite::_LUA_SPRITE_ = nullptr;

void LuaSprite::LoadSpriteSystem()
{
	_LUA_SPRITE_ = new LuaSprite();
	LuaSprite::LoadSpriteFunction();
}

void LuaSprite::LoadSpriteFunction()
{
	auto L = LStateManager::GetLuaState();
	using namespace luabridge;
	//getGlobalNamespace(L).addFunction("printMessage", printMessage);
	getGlobalNamespace(L)
		//.addFunction("createSprite", createSprite)
		.beginClass <::Vec2i>("Vec2i")
			.addConstructor <void(*) (int, int)>()
			.addData("x", &::Vec2i::x)
			.addData("y", &::Vec2i::y)
		.endClass()
		.beginClass<::Sprite>("Sprite")
			.addStaticFunction("createSprite", &Sprite::Create)
		.endClass();
	//.endNamespace();


	lua_pushcfunction(L, LuaSprite::CreateSprite);
	lua_setglobal(L, "createSprite");

	lua_pushcfunction(L, LuaSprite::SetSpritePosition);
	lua_setglobal(L, "setSpritePosition");
	lua_pushcfunction(L, LuaSprite::SetSpritePosition);
	lua_setglobal(L, "setDrawablePosition");

	lua_pushcfunction(L, LuaSprite::GetSpritePosition);
	lua_setglobal(L, "getSpritePosition");
	lua_pushcfunction(L, LuaSprite::GetSpritePosition);
	lua_setglobal(L, "getDrawablePosition");

	lua_pushcfunction(L, LuaSprite::DeleteSprite);
	lua_setglobal(L, "deleteSprite");
	lua_pushcfunction(L, LuaSprite::DeleteSprite);
	lua_setglobal(L, "deleteDrawable");

	lua_pushcfunction(L, LuaSprite::RotateSprite);
	lua_setglobal(L, "rotateSprite");
	lua_pushcfunction(L, LuaSprite::RotateSprite);
	lua_setglobal(L, "rotateDrawable");

	lua_pushcfunction(L, LuaSprite::GetSpriteRotation);
	lua_setglobal(L, "getSpriteRotation");
	lua_pushcfunction(L, LuaSprite::GetSpriteRotation);
	lua_setglobal(L, "getDrawableRotation");

	lua_pushcfunction(L, LuaSprite::FlipSpriteHorizental);
	lua_setglobal(L, "flipSpriteHorizontal");
	lua_pushcfunction(L, LuaSprite::FlipSpriteHorizental);
	lua_setglobal(L, "flipDrawableHorizontal");

	lua_pushcfunction(L, LuaSprite::FlipSpriteVertical);
	lua_setglobal(L, "flipSpriteVertical");
	lua_pushcfunction(L, LuaSprite::FlipSpriteVertical);
	lua_setglobal(L, "flipDrawableVertical");

	lua_pushcfunction(L, LuaSprite::SetSpriteRotationCenter);
	lua_setglobal(L, "setSpriteRotationCenter");
	lua_pushcfunction(L, LuaSprite::SetSpriteRotationCenter);
	lua_setglobal(L, "setDrawableRotationCenter");

	lua_pushcfunction(L, LuaSprite::GetSpriteSize);
	lua_setglobal(L, "getSpriteSize");
	lua_pushcfunction(L, LuaSprite::GetSpriteSize);
	lua_setglobal(L, "getDrawableSize");

	lua_pushcfunction(L, LuaSprite::AttachSpriteTo);
	lua_setglobal(L, "attachSpriteTo");
	lua_pushcfunction(L, LuaSprite::AttachSpriteTo);
	lua_setglobal(L, "attachDrawableTo");

	lua_pushcfunction(L, LuaSprite::SetSpriteVisisble);
	lua_setglobal(L, "setSpriteVisible");
	lua_pushcfunction(L, LuaSprite::SetSpriteVisisble);
	lua_setglobal(L, "setDrawableVisible");

	lua_pushcfunction(L, LuaSprite::IsSpriteVisisble);
	lua_setglobal(L, "isSpriteVisible");
	lua_pushcfunction(L, LuaSprite::IsSpriteVisisble);
	lua_setglobal(L, "isDrawableVisible");

	lua_pushcfunction(L, LuaSprite::Physicalize);
	lua_setglobal(L, "physicalize");

	lua_pushcfunction(L, LuaSprite::PhysicalizeWithOffset);
	lua_setglobal(L, "physicalizeWithOffset");

	lua_pushcfunction(L, LuaSprite::SetZOrder);
	lua_setglobal(L, "setZOrder");

	lua_pushcfunction(L, LuaSprite::getBody);
	lua_setglobal(L, "getBody");

	lua_pushcfunction(L, LuaSprite::getObjectsByType);
	lua_setglobal(L, "getObjectsByType");
}

LuaSprite* LuaSprite::GetSpriteManager()
{
	return _LUA_SPRITE_;
}

int LuaSprite::CreateSprite(lua_State *L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "snnn")) {
		return 1;
	}
	const char* path = lua_tostring(L, 1);  // get argument 
	int w = (int)lua_tonumber(L, 2);
	int h = (int)lua_tonumber(L, 3);
	int x = (int)lua_tonumber(L, 4);
	int y = (int)lua_tonumber(L, 5);
	auto obj = Sprite::Create(path, Vec2i(w, h), Vec2i(x, y));
	lua_pushlightuserdata(L, (void*)obj);
	return 1;
}

int LuaSprite::SetSpritePosition(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnn")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	sprt->setPosition(Vec2i(x, y));
	return 0;
}

int LuaSprite::GetSpritePosition(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	Vec2i p = sprt->getPosition();
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushinteger(L, p.x);
		lua_pushinteger(L, p.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::DeleteSprite(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		FREE(sprt);
	}
	return 0;
}

int LuaSprite::RotateSprite(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "ln")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	int rot = (int)lua_tonumber(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sprt->Rotate(rot);
	}
	return 0;
}

int LuaSprite::FlipSpriteHorizental(lua_State* L)
{
	/*Variable v[2];
	ErrorManager::GetErrorManager()->ParseArgs<Drawable*, bool>(L, "bl", v, NULL, true);
	v[0].Get<Drawable>().Flip(v[1].Get<bool>() ? FLIPTYPE::HORIZONTAL : FLIPTYPE::NONE);*/

	/*	Function f = FUNCTION(v[0]->Flip);

	f(v[1]);*/

	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lb")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sprt->Flip(bFlip ? FLIPTYPE::HORIZONTAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaSprite::FlipSpriteVertical(lua_State* L)
{
	/*Variable v[2];
	ErrorManager::GetErrorManager()->ParseArgs<Drawable*, bool>(L, "bl", v, NULL, true);
	v[0].Get<Drawable>().Flip(v[1].Get<bool>(true) ? FLIPTYPE::VERTICAL : FLIPTYPE::NONE);
	return 0;*/
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lb")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sprt->Flip(bFlip ? FLIPTYPE::VERTICAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaSprite::SetSpriteRotationCenter(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnn")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sprt->SetRotationCenter(Vec2i(x, y));
	}
	return 0;
}

int LuaSprite::GetSpriteSize(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	Vec2i sz;
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sz = sprt->getSize();
		lua_pushnumber(L, sz.x);
		lua_pushnumber(L, sz.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaEngine::LuaSprite::SetSpriteVisisble(lua_State * L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lb")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	bool isVis = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		sprt->setRender(isVis);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::GetSpriteFilename(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushstring(L, sprt->getFileName().c_str());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::GetSpriteRotation(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushnumber(L, sprt->GetRotation());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::AttachSpriteTo(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "llnn")) {
		return 1;
	}
	Drawable* toBeAttached = (Drawable*)lua_touserdata(L, 1);
	Drawable* AttachedTo = (Drawable*)lua_touserdata(L, 2);
	float offsetX = (float)lua_tonumber(L, 3);
	float offsetY = (float)lua_tonumber(L, 4);
	if (toBeAttached != NULL && AttachedTo != NULL) {
		toBeAttached->attachTo(AttachedTo, Vec2f(offsetX, offsetY));
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::IsSpriteVisisble(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* sprt = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), sprt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushboolean(L, sprt->isRender());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}


//THIS IS FRO DRAWABLE
int LuaSprite::Physicalize(lua_State* L) 
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnnnbss")) {
		return 1;
	}
	Drawable* phySprt = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), phySprt) != ObjectHandler::GetObjectHandler()->end()) {
		float mass = (float)lua_tonumber(L, 2);
		float friction = (float)lua_tonumber(L, 3);
		float restitution = (float)lua_tonumber(L, 4);
		bool isSensor = (bool)lua_toboolean(L, 5);
		const char* bodyType = lua_tostring(L, 6);
		const char* bodyShape = lua_tostring(L, 7);
		Physics2D::BodyType rBodyType = Physics2D::BodyType::STATIC;
		Physics2D::BodyShape rBodyShape = Physics2D::BodyShape::BOX;
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
		auto body = phySprt->Physicalize(Physics2D::BodyParams{ mass, friction, restitution, isSensor }, rBodyType, rBodyShape);
		lua_pushlightuserdata(L, (void*)body);
		return 1;
	}
	return 0;
}

int LuaSprite::PhysicalizeWithOffset(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "lnnnbssnn")) {
		return 1;
	}
	Drawable* phySprt = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), phySprt) != ObjectHandler::GetObjectHandler()->end()) {
		float mass = (float)lua_tonumber(L, 2);
		float friction = (float)lua_tonumber(L, 3);
		float restitution = (float)lua_tonumber(L, 4);
		bool isSensor = (float)lua_tonumber(L, 5);
		const char* bodyType = lua_tostring(L, 6);
		const char* bodyShape = lua_tostring(L, 7);
		Physics2D::BodyShape rBodyShape = Physics2D::BodyShape::BOX;
		Physics2D::BodyType rBodyType = Physics2D::BodyType::STATIC;
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
		float offx = (float)lua_tonumber(L, 8);
		float offy = (float)lua_tonumber(L, 9);
		auto body = phySprt->Physicalize(Physics2D::BodyParams{mass, friction, restitution, isSensor}, rBodyType, rBodyShape,Vec2f(offx, offy));
		lua_pushlightuserdata(L, (void*)body);
		return 1;
	}
	return 0;
}

int LuaSprite::SetZOrder(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "ln")) {
		return 1;
	}
	Drawable* drawable = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), drawable) != ObjectHandler::GetObjectHandler()->end()) {
		int zorder = (int)lua_tonumber(L, 2);
		drawable->setZOrder(zorder);
		return 1;
	}
	return 0;
}

int LuaSprite::getBody(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "l")) {
		return 1;
	}
	Drawable* drawable = (Drawable*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), drawable) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushlightuserdata(L, (void*)drawable->getBody());
		return 1;
	}
	return 0;
}

void l_pushtabledrawable(lua_State* L, int key, void* value) {
	lua_pushinteger(L, key);
	lua_pushlightuserdata(L, value);
	lua_settable(L, -3);
}

int LuaSprite::getObjectsByType(lua_State* L)
{
	if (!ErrorManager::GetErrorManager()->isValidArgument(L, "s")) {
		return 1;
	}
	const char* type = lua_tostring(L, 1);
	int index = 0;
	lua_newtable(L);
	if (strcmp(type, "Sprite") == 0) {
		for (auto& itr : *ObjectHandler::GetObjectHandler())
		{
			auto sprite = dynamic_cast<Sprite*>(itr);
			if (sprite != nullptr) {
				l_pushtabledrawable(L, index + 1, (void*)sprite);
				index++;
			}
		}
	}else if (strcmp(type, "Animation") == 0) {
		for (auto& itr : *ObjectHandler::GetObjectHandler())
		{
			auto anim = dynamic_cast<Animation*>(itr);
			if (anim != nullptr) {
				l_pushtabledrawable(L, index + 1, (void*)anim);
				index++;
			}
		}
	}else if (strcmp(type, "Text") == 0) {
		for (auto& itr : *ObjectHandler::GetObjectHandler())
		{
			auto anim = dynamic_cast<UI::Text*>(itr);
			if (anim != nullptr) {
				l_pushtabledrawable(L, index + 1, (void*)anim);
				index++;
			}
		}
	}
	return 1;
}