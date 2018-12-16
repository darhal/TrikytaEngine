#pragma once
#include <list>

namespace UI {
	class Manager
	{
	public:
		typedef std::list<class Base*> ObjectsVec;

		static Manager* InitManager();
		static Manager* getManager();
		static class EventHandler* getEventManager();
		static ObjectsVec& getUIContainer();
		static void addElement(class Base*, bool=false);
		static void removeElement(class Base*, bool);
		static ObjectsVec& getUIRenderableContainer();
		static void renderElements(float);
		static ObjectsVec& GetQueueToDelete();
	private:
		Manager() {}
		static Manager* _Manager;
		static class EventHandler* _EventManager;
		ObjectsVec m_UIContainer;
		ObjectsVec m_UIRenderableContainer;
		ObjectsVec m_QueueToRemove;
	};
}