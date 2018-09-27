#include "LStateManager/LStateManager.h"
#include <UI/UIText.h>
#include <lua.hpp>
#include "LuaText.h"
#include <core/Objects/ObjectHandler.h>


using namespace UI;
using namespace LuaEngine;

LuaText* LuaText::_LUA_TEXT_ = nullptr;

void LuaText::LoadTextSystem()
{
	if (_LUA_TEXT_ == nullptr) {
		_LUA_TEXT_ = new LuaText();
	}
	LuaText::LoadTextFunction();
}

LuaText* LuaText::GetTextManager()
{
	return _LUA_TEXT_;
}

void LuaText::LoadTextFunction()
{
	auto L = LStateManager::GetLuaState();

	lua_pushcfunction(L, LuaText::createText);
	lua_setglobal(L, "createText");

	lua_pushcfunction(L, LuaText::setTextColor);
	lua_setglobal(L, "setTextColor");

	lua_pushcfunction(L, LuaText::SetTextPosition);
	lua_setglobal(L, "setTextPosition");

	lua_pushcfunction(L, LuaText::GetTextPosition);
	lua_setglobal(L, "getTextPosition");

	lua_pushcfunction(L, LuaText::DeleteText);
	lua_setglobal(L, "deleteText");

	lua_pushcfunction(L, LuaText::RotateText);
	lua_setglobal(L, "rotateText");

	lua_pushcfunction(L, LuaText::GetTextRotation);
	lua_setglobal(L, "getTextRotation");

	lua_pushcfunction(L, LuaText::FlipTextHorizental);
	lua_setglobal(L, "flipTextHorizental");

	lua_pushcfunction(L, LuaText::FlipTextVertical);
	lua_setglobal(L, "flipTextVertical");

	lua_pushcfunction(L, LuaText::SetTextRotationCenter);
	lua_setglobal(L, "setTextRotationCenter");

	lua_pushcfunction(L, LuaText::GetTextSize);
	lua_setglobal(L, "getTextSize");

	lua_pushcfunction(L, LuaText::AttachTextTo);
	lua_setglobal(L, "attachTextTo");

	lua_pushcfunction(L, LuaText::SetTextVisisble);
	lua_setglobal(L, "setTextVisible");

	lua_pushcfunction(L, LuaText::IsTextVisisble);
	lua_setglobal(L, "isTextVisible");

	lua_pushcfunction(L, LuaText::setText);
	lua_setglobal(L, "setText");
}

int LuaText::createText(lua_State* L)
{
	
	lua_pushlightuserdata(L,
		(void*)UI::Text::createText(
			lua_tostring(L, 1), lua_tostring(L, 2), (uint8)lua_tonumber(L, 3),
			Vec2i((int)lua_tonumber(L, 4), (int)lua_tonumber(L, 5)),
			Color{ (uint8)lua_tonumber(L, 6), (uint8)lua_tonumber(L, 7),(uint8)lua_tonumber(L, 8), 255 }
		)
	);
	return 1;
}

int LuaText::setTextColor(lua_State* L)
{
	UI::Text* txt = (UI::Text*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->setColor(Color{ (uint8)lua_tonumber(L, 2), (uint8)lua_tonumber(L, 3), (uint8)lua_tonumber(L, 4), 255 });
	}
	return 0;
}

int LuaText::setText(lua_State* L)
{
	UI::Text* txt = (UI::Text*)lua_touserdata(L, 1);
	auto text = lua_tostring(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->updateText(text);
	}
	return 0;
}


int LuaText::SetTextPosition(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	txt->setPosition(Vec2i(x, y));
	return 0;
}

int LuaText::GetTextPosition(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	Vec2i p = txt->getPosition();
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushinteger(L, p.x);
		lua_pushinteger(L, p.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaText::DeleteText(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		FREE(txt);
	}
	return 0;
}

int LuaText::RotateText(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	int rot = (int)lua_tonumber(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->Rotate(rot);
	}
	return 0;
}

int LuaText::FlipTextHorizental(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->Flip(bFlip ? FLIPTYPE::HORIZONTAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaText::FlipTextVertical(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	int bFlip = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->Flip(bFlip ? FLIPTYPE::VERTICAL : FLIPTYPE::NONE);
	}
	return 0;
}

int LuaText::SetTextRotationCenter(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	int x = (int)lua_tonumber(L, 2);
	int y = (int)lua_tonumber(L, 3);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->SetRotationCenter(Vec2i(x, y));
	}
	return 0;
}

int LuaText::GetTextSize(lua_State* L)
{
	Drawable* txt = (Drawable*)lua_touserdata(L, 1);
	Vec2i sz;
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		sz = txt->getSize();
		lua_pushnumber(L, sz.x);
		lua_pushnumber(L, sz.y);
		return 2;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaEngine::LuaText::SetTextVisisble(lua_State * L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	bool isVis = lua_toboolean(L, 2);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		txt->setRender(isVis);
		lua_pushboolean(L, true);
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

/*int LuaText::GetTextFilename(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushstring(L, txt->().c_str());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}*/

int LuaText::GetTextRotation(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushnumber(L, txt->GetRotation());
		return 1;
	}
	lua_pushboolean(L, false);
	return 1;
}

int LuaText::AttachTextTo(lua_State* L)
{
	Text* toBeAttached = (Text*)lua_touserdata(L, 1);
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

int LuaText::IsTextVisisble(lua_State* L)
{
	Text* txt = (Text*)lua_touserdata(L, 1);
	if (std::find(ObjectHandler::GetObjectHandler()->begin(), ObjectHandler::GetObjectHandler()->end(), txt) != ObjectHandler::GetObjectHandler()->end()) {
		lua_pushboolean(L, txt->isRender());
		return 1;
	}
	lua_pushnil(L);
	return 1;
}