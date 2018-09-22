#pragma once
#include <memory>
#include <vector>
#include <list>


//typedef std::vector<class Object*> ObjectsVec;
typedef std::list<class Object*> ObjectsVec;

class ObjectHandler 
{
public:
	
	static ObjectHandler* GetObjectManager();
	static ObjectsVec* GetObjectHandler();
	static void PushObject(class Object*);
	static void RemoveObject(Object* p_Obj);
private :
	static ObjectHandler* _ObjectHandler;
	ObjectHandler() : m_ObjectHandler(std::make_shared<ObjectsVec>())
	{}
	std::shared_ptr<ObjectsVec> m_ObjectHandler;
};
