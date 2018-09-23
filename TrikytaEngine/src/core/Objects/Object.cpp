#include <core/Common/TrikytaEngine.h>
#include "Object.h"
#include "ObjectHandler.h"


Object::Object(Vec2i m_Pos, Vec2i p_Size) :
	m_Childrens(new std::vector<Object*>), m_Size(p_Size), m_Position(m_Pos)
{
	ObjectHandler::PushObject(this);
	m_IsVisible = true;
};

Object::~Object()
{
	ObjectHandler::RemoveObject(this);
};

bool Object::init()
{
	return true;
}

void Object::setVisible(bool isVisible)
{
	if (isVisible && !m_IsVisible) {
		ObjectHandler::SetObjectSleeping(this, isVisible);
		m_IsVisible = isVisible;
	}else if(!isVisible && m_IsVisible) {
		ObjectHandler::SetObjectSleeping(this, isVisible);
		m_IsVisible = isVisible;
	}
}

void Object::render(float dt)
{
	if (m_Parent != nullptr)
	{
		m_Position.x = m_Parent->getPosition().x + (int)(m_Offset.x*m_Parent->getSize().x);
		m_Position.y = m_Parent->getPosition().y + (int)(m_Offset.y*m_Parent->getSize().y);
	}
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
	m_Parent = obj;
	m_Position.x = obj->getPosition().x;
	m_Position.y = obj->getPosition().y;
	obj->addChildren(this);
	m_Offset = p_Offset;
}