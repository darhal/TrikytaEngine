#include <core/Common/defines.h>
#include "ObjectHandler.h"
#include "Object.h"

ObjectHandler* ObjectHandler::_ObjectHandler = 0;

ObjectHandler* ObjectHandler::GetObjectManager()
{
	if (_ObjectHandler == nullptr)
	{
		_ObjectHandler = new ObjectHandler();
	}
	return _ObjectHandler;
}

ObjectsVec* ObjectHandler::GetObjectHandler()
{
	return GetObjectManager()->m_ObjectHandler.get();
}

void ObjectHandler::PushObject(Object* p_Obj)
{
	GetObjectHandler()->emplace_back(p_Obj);
	p_Obj->m_Manager_Index = GetObjectHandler()->size() - 1;
	Log("Creating object : %p at %d", p_Obj, p_Obj->m_Manager_Index);
}
 
void ObjectHandler::RemoveObject(Object* p_Obj)
{
	Log("Deleted object : %p at %d", p_Obj, p_Obj->m_Manager_Index);
	GetObjectHandler()->remove(p_Obj);
}