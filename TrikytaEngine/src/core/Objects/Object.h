#pragma once
#include <core/Common/Vec2.h>
#include <vector>
#include "IObject.h"


class Object : public IObject
{
public:
	/*static Object* Create()
	{
		TrikytaEngine::getEngine()->getObjectHandler()->emplace_back(new Object());
		TrikytaEngine::getEngine()->getObjectHandler()->back()->m_Manager_Index = TrikytaEngine::getEngine()->getObjectHandler()->size() - 1;
		Log("Creating object : %p at %d", (TrikytaEngine::getEngine()->getObjectHandler()->back()), TrikytaEngine::getEngine()->getObjectHandler()->back()->m_Manager_Index);
		return TrikytaEngine::getEngine()->getObjectHandler()->back();
	}*/

	Object(Vec2i m_Pos = Vec2i(0, 0), Vec2i p_Size = Vec2i(0, 0));
	virtual ~Object();

	virtual bool init();
	virtual void render(float);

	virtual Vec2<int> getSize() { return m_Size; }
	virtual Vec2<int> getPosition() { return m_Position; }

	void addChildren(Object*);
	void attachTo(Object*, Vec2f = Vec2f(0.f, 0.f));

	inline std::vector<Object*>* getChildrens() const { return m_Childrens; }


	int m_Manager_Index;
protected:
	Vec2<int> m_Size;
	Vec2<int> m_Position;
	Vec2f m_Offset;
	Object* m_Parent;
	std::vector<Object*>* m_Childrens;
private:

};
