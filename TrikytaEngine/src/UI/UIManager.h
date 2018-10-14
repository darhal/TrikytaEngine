#pragma once
#include <vector>

namespace UI {
	class Manager
	{
	public:
		typedef std::vector<class Base*> ObjectsVec;

		static Manager* InitManager();
		static Manager* getManager();
		static class EventHandler* getEventManager();
		static ObjectsVec& getUIContainer();
		static void addElement(class Base*);
		static void removeElement(class Base*);
	private:
		Manager() {}
		static Manager* _Manager;
		static class EventHandler* _EventManager;
		ObjectsVec m_UIContainer;
	};
}