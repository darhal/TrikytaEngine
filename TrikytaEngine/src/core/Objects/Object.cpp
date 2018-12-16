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
			FREE(component.second);
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
	}else if(!isVisible && m_IsRender) {
		ObjectHandler::SetObjectSleeping(this, isVisible);
	}
	m_IsRender = isVisible;
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

const Component* Object::getComponent(unsigned int compType)
{
	if (m_Components[compType]) {
		return static_cast<Component*>(m_Components[compType]);
	}
	return NULL;
}

void Object::removeComponent(Component* component)
{
	if (m_Components.count(component->getComponentType()) > 0 && m_Components[component->getComponentType()]) {
		m_Components[component->getComponentType()] = NULL;
		FREE(component);
	}
}

void Object::getComponents(std::vector<Component*>& out_vector)
{
	out_vector.reserve(m_Components.size());
	for (auto& itr : m_Components) {
		out_vector.emplace_back(static_cast<Component*>(itr.second));
	}
}