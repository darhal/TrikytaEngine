#pragma once
#include "core/Objects/IObject.h"

#define REGISTER_COMPONENT(__class__, __component_type__)\
	static int getType(){ \
		return __component_type__; \
	} \

enum {
	BASIC_COMPONENT,
	POSITION_COMPONENT,
	CAMERA_COMPONENT,
	TILE_COMPONENT,
};

class Object;

class Component : public IObject
{
public:
	REGISTER_COMPONENT(Component, BASIC_COMPONENT);

	Component() : m_ComponentType(BASIC_COMPONENT){}
	virtual ~Component(){}
	int getComponentType() { return m_ComponentType; }
	Object* getOwner() { return m_Owner; }
	void setOwner(Object* owner) { m_Owner = owner; }
protected:
	int m_ComponentType;
	Object* m_Owner;
};