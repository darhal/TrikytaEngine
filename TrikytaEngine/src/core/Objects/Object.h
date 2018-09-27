#pragma once
#include <core/Common/Vec2.h>
#include "IObject.h"
#include <vector>


class Object : public IObject
{
public:
	Object(bool=true);
	virtual ~Object();

	virtual bool init();
	virtual void render(float);

	virtual double GetRotation() { return 0.f; };

	void addChildren(Object*);
	void attachTo(Object*, Vec2f = Vec2f(0.f, 0.f));

	void setRender(bool isVisible);

	inline std::vector<Object*>* getChildrens() const { return m_Childrens; }
	inline bool isRender() {return m_IsRender;}

	int m_Manager_Index;
	virtual Vec2i getPosition() { return Vec2i(0, 0); };
protected:
	bool m_IsRender;

	Object* m_Parent = nullptr;
	std::vector<Object*>* m_Childrens;
private:

};
