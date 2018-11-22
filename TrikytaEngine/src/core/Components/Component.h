#pragma once
#include "core/Objects/IObject.h"

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
	Component() :
		m_ComponentType(BASIC_COMPONENT)
	{}

	virtual int getComponentType() { return m_ComponentType; }
	virtual Object* getOwner() { return m_Owner; }
	virtual void setOwner(Object* owner) { m_Owner = owner; }
protected:
	int m_ComponentType;
	Object* m_Owner;
};