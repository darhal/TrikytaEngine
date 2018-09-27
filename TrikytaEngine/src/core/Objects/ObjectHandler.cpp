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
	GetObjectHandler()->push_back(p_Obj);
	p_Obj->m_Manager_Index = GetObjectHandler()->size() - 1;
	LogInfoConsole("Creating object : %p at %d", p_Obj, p_Obj->m_Manager_Index);
}
 
void ObjectHandler::RemoveObject(Object* p_Obj)
{
	LogInfoConsole("Deleted object : %p at %d", p_Obj, p_Obj->m_Manager_Index);
	GetObjectHandler()->remove(p_Obj);
}

void ObjectHandler::SetObjectSleeping(Object* p_Obj, bool isSleep)
{
	if	(!isSleep) {
		GetSleepingObjects()->push_back(p_Obj);
		GetObjectHandler()->remove(p_Obj);
	}else {
		GetObjectHandler()->push_back(p_Obj);
		GetSleepingObjects()->remove(p_Obj);
	}
}

ObjectsVec * ObjectHandler::GetSleepingObjects()
{
	return GetObjectManager()->m_SleepObjects.get();
}

void ObjectHandler::PushObjectAsSleep(Object* p_Obj)
{
	GetSleepingObjects()->push_back(p_Obj);
}

