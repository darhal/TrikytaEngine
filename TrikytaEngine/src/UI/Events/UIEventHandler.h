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
		EventHandler() :m_BlockProcess(false) {};
		void BlockEvents(bool);
		void processEvents(SDL_Event&);
	private:
		bool m_BlockProcess;
	};
}