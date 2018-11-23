#include <core/Common/TrikytaEngine.h>
#include "Object.h"
#include "ObjectHandler.h"
#include "core/Drawable/Drawable.h"
#include "core/Components/Component.h"
#include <algorithm>

Object::Object(bool pRegisterInHandler)
{
	m_Childrens = new std::vector<Object*>;
	if (pRegisterInHandler) {
		ObjectHandler::PushObject(this);
	}else{
		ObjectHandler::PushObjectAsSleep(this);
	}
	m_IsRender = pRegisterInHandler;
	m_AutoClearComponent = true;
};

Object::~Object()
{
	ObjectHandler::RemoveObject(this);
	//Clear components !
	if (m_AutoClearComponent) {
		for (auto component : m_Components) {
			FREE(component);
		}
	}
	m_Components.clear();
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

void Object::render(float /*dt*/)
{
}

void Object::addChildren(Object* obj)
{
	if (obj == nullptr) { LogConsole(MESSAGE_TYPE::ERROR,  "Attempt to add null as a child!") return; }
	m_Childrens->push_back(obj);
}

void Object::attachTo(Object* obj, Vec2f p_Offset)
{
	if (obj == nullptr) {
		LogConsole(MESSAGE_TYPE::ERROR,"Attempt to attach to null object!")
		return;
	}
	obj->addChildren(this);
	m_Parent = obj;
}

const Component* Object::getComponent(int compType)
{
	for (const auto& component : m_Components) {
		if (component->getComponentType() == compType) {
			return component;
		}
	}
	return NULL;
}

void Object::addComponent(Component* component)
{
	if (component != NULL) {
		m_Components.emplace_back(component);
		component->setOwner(this);
		return;
	}

}

void Object::removeComponent(Component* component)
{
	std::remove_if(m_Components.begin(), m_Components.end(), [=](const Component* o) { return o == component; }), m_Components.end();
	FREE(component);
}

const std::vector<Component*>& Object::getComponents()
{
	return m_Components;
}