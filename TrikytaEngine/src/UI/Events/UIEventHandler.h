#pragma once
#include <functional>
#include "core/Common/defines.h"

union SDL_Event;

namespace UI {
	class EventHandler
	{
	public:
		using Events = ::Events;
		static EventHandler* InitEventManager();

		void processEvents(SDL_Event&);

		typedef std::function<void(union SDL_Event&)> OnEditBoxChangeType;
		/*template <Events EventType, typename Func,
			typename std::enable_if<EventType == ON_EDITBOX_CHANGE, bool>::type = true>
			void addEventHandler(Func&& func)
		{
			m_OnEditBoxChangeCallbacks.push_back(OnEditBoxChangeType(std::forward<Func>(func)));
		}

		template <Events EventType, typename Func,
			typename std::enable_if<EventType == ON_EDITBOX_CHANGE, bool>::type = true>
			void removeEventHandler(Func&& func)
		{
			//std::find(m_OnEditBoxChangeCallbacks->begin(), m_OnEditBoxChangeCallbacks->end(), OnEditBoxChangeType(std::forward<Func>(func)))
			m_OnEditBoxChangeCallbacks.pop_back();
		}*/
	private:
		//UI Events:
		std::vector<OnEditBoxChangeType> m_OnEditBoxChangeCallbacks;
	};
}