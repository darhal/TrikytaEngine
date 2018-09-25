#include <lua.hpp>
#include "LuaAnimation.h"
#include <core/Drawable/Animation.h>
#include "LStateManager/LStateManager.h"
#include "Shared/ldefines.h"
#include <core/Objects/ObjectHandler.h>

using namespace LuaEngine;

LuaAnimation* LuaAnimation::_LUA_ANIMATION_ = nullptr;

void LuaAnimation::LoadAnimationSystem()
{
	_LUA_ANIMATION_ = new LuaAnimation();
	LoadAnimationFunctions();
}

LuaAnimation* LuaAnimation::GetAnimationManager()
{
	return _LUA_ANIMATION_;
}

void LuaAnimation::LoadAnimationFunctions()
{
	auto L = LStateManager::GetLuaState();

	lua_pushcfunction(L, LuaAnimation::CreateAnimation);
	lua_setglobal(L, "createAnimation");

	lua_pushcfunction(L, LuaAnimation::SetAnimationPosition);
	lua_setglobal(L, "setAnimationPosition");

	lua_pushcfunction(L, LuaAnimation::GetAnimationPosition);
	lua_setglobal(L, "getAnimationPosition");

	lua_pushcfunction(L, LuaAnimation::DeleteAnimation);
	lua_setglobal(L, "deleteAnimation");

	lua_pushcfunction(L, LuaAnimation::RotateAnimation);
	lua_setglobal(L, "rotateAnimation");

	lua_pushcfunction(L, LuaAnimation::FlipAnimationHorizental);
	lua_setglobal(L, "flipAnimationHorizental");

	lua_pushcfunction(L, LuaAnimation::FlipAnimationVertical);
	lua_setglobal(L, "flipAnimationVertical");

	lua_pushcfunction(L, LuaAnimation::SetAnimationRotationCenter);
	lua_setglobal(L, "setAnimationRotationCenter");

	lua_pushcfunction(L, LuaAnimation::GetAnimationSize);
	lua_setglobal(L, "getAnimationSize");

	lua_pushcfunction(L, LuaAnimation::GetAnimationRotation);
	lua_setglobal(L, "getAnimationRotation");

	lua_pushcfunction(L, LuaAnimation::AttachAnimationTo);
	lua_setglobal(L, "attachAnimationTo");

	lua_pushcfunction(L, LuaAnimation::SetAnimationVisisble);
	lua_setglobal(L, "setAnimationVisible");

	lua_pushcfunction(L, LuaAnimation::IsAnimationVisisble);
	lua_setglobal(L, "isAnimationVisible");
}

int LuaAnimation::CreateAnimation(lua_State* L)
{
	const char* spritePath = lua_tostring(L, 1); 
	const char* parserPath = lua_tostring(L, 2);  
	int w = (int)lua_tonumber(L, 3);
	int h = (int)lua_tonumber(L, 4);
	int x = (int)lua_tonumber(L, 5);
	int y = (int)lua_tonumber(L, 6);
	float timeToUpdate = (float)lua_tonumber(L, 7);
	LogL("Info", "Path : %s, parser: %s, w:%d, h:%d, x:%d, y:%d", spritePath, parserPath, w, h, x, y);
	auto obj = Animation::Create(spritePath, parserPath, Vec2i(w, h), Vec2i(x, y), timeToUpdate);
	lua_pushlightuserdata(L, (void*)obj);
	return 1;
}

int LuaAnimation::SetAnimationPosition(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	anim->setPosition(Vec2i(x, y));
	return 0;
}

int LuaAnimation::GetAnimationPosition(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	Vec2i p = anim->getPosition();
	if (anim != NULL) {
		lua_pushinteger(L, p.x);
		lua_pushinteger(L, p.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaAnimation::DeleteAnimation(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), anim) != ObjectHandler::GetObjectHandler()->end()) {
		FREE(anim);
	}
	return 0;
}

int LuaAnimation::RotateAnimation(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	int rot = (int)lua_tonumber(L, 2);
	if (anim != NULL) {
		anim->Rotate(rot);
	}
	return 0;
}

int LuaAnimation::FlipAnimationHorizental(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (anim != NULL) {
		anim->Flip(bFlip ? FLIPTYPE::HORIZONTAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaAnimation::FlipAnimationVertical(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (anim != NULL) {
		anim->Flip(bFlip ? FLIPTYPE::VERTICAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaAnimation::SetAnimationRotationCenter(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (anim != NULL) {
		anim->SetRotationCenter(Vec2i(x, y));
	}
	return 0;
}

int LuaAnimation::GetAnimationSize(lua_State* L)
{
	Drawable* anim = (Drawable*)lua_touserdata(L, 1);
	Vec2i sz;
	if (anim != NULL) {
		sz = anim->getSize();
		lua_pushnumber(L, sz.x);
		lua_pushnumber(L, sz.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaAnimation::GetAnimationFilename(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	if (anim != NULL) {
		lua_pushstring(L, anim->getFileName().c_str());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaAnimation::AttachAnimationTo(lua_State* L)
{
	Animation* toBeAttached = (Animation*)lua_touserdata(L, 1);
	Animation* AttachedTo = (Animation*)lua_touserdata(L, 2);
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

int LuaAnimation::GetAnimationRotation(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	if (anim != NULL) {
		lua_pushnumber(L, anim->GetRotation());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaAnimation::IsAnimationVisisble(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	if (anim != NULL) {
		lua_pushboolean(L, anim->isRender());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

int LuaAnimation::SetAnimationVisisble(lua_State* L)
{
	Animation* anim = (Animation*)lua_touserdata(L, 1);
	bool isVis = lua_toboolean(L, 2);
	if (anim != NULL) {
		anim->setRender(isVis);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}