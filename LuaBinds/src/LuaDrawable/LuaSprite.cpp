#include <lua.hpp>
#include "LuaSprite.h"
#include <core/Drawable/Sprite.h>
#include "LStateManager/LStateManager.h"
#include <core/Common/Vec2.h>

#include "Shared/ldefines.h"

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
	
	lua_pushcfunction(L, LuaSprite::CreateSprite);
	lua_setglobal(L, "createSprite");

	lua_pushcfunction(L, LuaSprite::SetSpritePosition);
	lua_setglobal(L, "setSpritePosition");

	lua_pushcfunction(L, LuaSprite::GetSpritePosition);
	lua_setglobal(L, "getSpritePosition");

	lua_pushcfunction(L, LuaSprite::DeleteSprite);
	lua_setglobal(L, "deleteSprite");

	lua_pushcfunction(L, LuaSprite::RotateSprite);
	lua_setglobal(L, "rotateSprite");

	lua_pushcfunction(L, LuaSprite::GetSpriteRotation);
	lua_setglobal(L, "getSpriteRotation");

	lua_pushcfunction(L, LuaSprite::FlipSpriteHorizental);
	lua_setglobal(L, "flipSpriteHorizental");

	lua_pushcfunction(L, LuaSprite::FlipSpriteVertical);
	lua_setglobal(L, "flipSpriteVertical");

	lua_pushcfunction(L, LuaSprite::SetSpriteRotationCenter);
	lua_setglobal(L, "setSpriteRotationCenter");

	lua_pushcfunction(L, LuaSprite::GetSpriteSize);
	lua_setglobal(L, "getSpriteSize");

	lua_pushcfunction(L, LuaSprite::AttachSpriteTo);
	lua_setglobal(L, "attachSpriteTo");

	lua_pushcfunction(L, LuaSprite::SetSpriteVisisble);
	lua_setglobal(L, "setSpriteVisible");

	lua_pushcfunction(L, LuaSprite::IsSpriteVisisble);
	lua_setglobal(L, "isSpriteVisible");
}

LuaSprite* LuaSprite::GetSpriteManager()
{
	return _LUA_SPRITE_;
}

int LuaSprite::CreateSprite(lua_State *L)
{
	const char* path = lua_tostring(L, -5);  // get argument 
	int w = (int)lua_tonumber(L, -4);
	int h = (int)lua_tonumber(L, -3);
	int x = (int)lua_tonumber(L, -2);
	int y = (int)lua_tonumber(L, -1);
	auto obj = Sprite::Create(path, Vec2i(w, h), Vec2i(x, y));
	lua_pushlightuserdata(L, (void*)obj);
	return 1;
}

int LuaSprite::SetSpritePosition(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	sprt->setPosition(Vec2i(x, y));
	return 0;
}

int LuaSprite::GetSpritePosition(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	Vec2i p = sprt->getPosition();
	if (sprt != NULL) {
		lua_pushinteger(L, p.x);
		lua_pushinteger(L, p.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::DeleteSprite(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	if (sprt != NULL) {
		FREE(sprt);
		sprt = NULL;
	}
	return 0;
}

int LuaSprite::RotateSprite(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int rot = (int)lua_tonumber(L, 2);
	if (sprt != NULL) {
		sprt->Rotate(rot);
	}
	return 0;
}

int LuaSprite::FlipSpriteHorizental(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (sprt != NULL) {
		sprt->Flip(bFlip ? FLIPTYPE::HORIZONTAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaSprite::FlipSpriteVertical(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (sprt != NULL) {
		sprt->Flip(bFlip ? FLIPTYPE::VERTICAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaSprite::SetSpriteRotationCenter(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (sprt != NULL) {
		sprt->SetRotationCenter(Vec2i(x, y));
	}
	return 0;
}

int LuaSprite::GetSpriteSize(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	Vec2i sz;
	if (sprt != NULL) {
		sz = sprt->getDrawSize();
		lua_pushnumber(L, sz.x);
		lua_pushnumber(L, sz.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaEngine::LuaSprite::SetSpriteVisisble(lua_State * L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	bool isVis = lua_toboolean(L, 2);
	if (sprt != NULL) {
		sprt->setVisible(isVis);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::GetSpriteFilename(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	if (sprt != NULL) {
		lua_pushstring(L, sprt->getFileName().c_str());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::GetSpriteRotation(lua_State* L)
{
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	if (sprt != NULL) {
		lua_pushnumber(L, sprt->GetRotation());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaSprite::AttachSpriteTo(lua_State* L)
{
	Sprite* toBeAttached = (Sprite*)lua_touserdata(L, 1);
	Sprite* AttachedTo = (Sprite*)lua_touserdata(L, 2);
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
	Sprite* sprt = (Sprite*)lua_touserdata(L, 1);
	if (sprt != NULL) {
		lua_pushboolean(L, sprt->isVisisble());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

/*typedef int (LuaSprite::*mem_func)(lua_State * L);

// This template wraps a member function into a C-style "free" function compatible with lua.
template <mem_func func>
int dispatch(lua_State * L) {
	LuaSprite * ptr = *static_cast<LuaSprite**>(lua_getextraspace(L));
	return ((*ptr).*func)(L);
}*/