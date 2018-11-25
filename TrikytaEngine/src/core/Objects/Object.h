#pragma once
#include <core/Common/Vec2.h>
#include "IObject.h"
#include <vector>
#include <map>
#include <algorithm>
#include "core/Components/Component.h"

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

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	T* getComponent() {
		if (T::getType() && m_Components.count(T::getType()) > 0 && m_Components[T::getType()]) {
			return static_cast<T*>(m_Components[T::getType()]);
		}
		return NULL;
	};

	template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
	void addComponent(T* component) {
		if (component != NULL && m_Components[component->getComponentType()] == NULL) { // dont overwrite components!
			m_Components[component->getComponentType()] = ((void*)component);
			component->setOwner(this);
			return;
		}
	};

	void removeComponent(Component* component);
	void getComponents(std::vector<Component*>&);
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
	std::map<int, void*> m_Components; // components !
private:

};

