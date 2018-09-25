#include <core/Common/TrikytaEngine.h>
#include "Object.h"
#include "ObjectHandler.h"
#include "core/Drawable/Drawable.h"


Object::Object() :
	m_Childrens(new std::vector<Object*>)
{
	ObjectHandler::PushObject(this);
	m_IsRender = true;
};

Object::~Object()
{
	ObjectHandler::RemoveObject(this);
};

bool Object::init()
{
	return true;
}

void Object::setRender(bool isVisible)
{
	if (isVisible && !m_IsRender) {
		ObjectHandler::SetObjectSleeping(this, isVisible);
		m_IsRender = isVisible;
	}else if(!isVisible && m_IsRender) {
		ObjectHandler::SetObjectSleeping(this, isVisible);
		m_IsRender = isVisible;
	}
}

void Object::render(float dt)
{
}

void Object::addChildren(Object* obj)
{
	if (obj == nullptr) { Log("Error attempt to add null as a child!") return; }
	m_Childrens->push_back(obj);
}


void Object::attachTo(Object* obj, Vec2f p_Offset)
{
	if (obj == nullptr) {
		Log("Error attempt to attach to null object!")
		return;
	}
	obj->addChildren(this);
	m_Parent = obj;
}