#pragma once
#include "core/Objects/IObject.h"
#include "core/Common/Macros.hpp"

class Object;

ADD_COMPONENT(BASIC_COMPONENT)
class Component : public IObject
{
public:
	REGISTER_COMPONENT(Component, BASIC_COMPONENT);
	virtual ~Component(){}
	Object* getOwner() { return m_Owner; }
	void setOwner(Object* owner) { m_Owner = owner; }
protected:
	Object* m_Owner;
};