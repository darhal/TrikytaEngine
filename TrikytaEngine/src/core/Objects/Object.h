#pragma once
#include <core/Common/Vec2.h>
#include "IObject.h"
#include <vector>
#include <algorithm>

class Component;

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

	template<typename T>
	T* getComponent() {
		T* componentResult = NULL;
		bool runNullPtrCollector = false;
		for (const auto& component : m_Components) {
			if (component == NULL) { runNullPtrCollector = true; continue; }
			auto potentialComponent = dynamic_cast<T*>(component);
			if (potentialComponent != NULL) {
				componentResult = potentialComponent;
				break;
			}
		}
		if (runNullPtrCollector) {std::remove_if(m_Components.begin(), m_Components.end(), [=](const Component* o) { return o == NULL; }), m_Components.end();}
		return componentResult;
	};
	void addComponent(Component* component);
	void removeComponent(Component* component);
	const std::vector<Component*>& getComponents();
	const Component* getComponent(int compType);

	void setRender(bool isVisible);

	inline std::vector<Object*>* getChildrens() const { return m_Childrens; }
	inline bool isRender() {return m_IsRender;}
	inline void autoClearComponents(bool b) { m_AutoClearComponent = b; }
	inline bool isComponenetAutoClear() { return m_AutoClearComponent; }

	int m_Manager_Index;
	virtual Vec2i getPosition() { return Vec2i(0, 0); };
protected:
	bool m_IsRender;
	bool m_AutoClearComponent;
	Object* m_Parent = nullptr;
	std::vector<Object*>* m_Childrens;
	std::vector<Component*> m_Components;
private:

};

