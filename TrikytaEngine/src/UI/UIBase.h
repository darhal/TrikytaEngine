#pragma once
#include "core/Common/Vec2.h"

union SDL_Event;

namespace UI {
	class Base
	{
	public:
		virtual void PorcessEvents(SDL_Event&);
		virtual void OnUIClick(Vec2i, bool) = 0;
		virtual void OnUIFocus(bool) = 0;
		virtual Vec2i getPos() = 0;
		virtual Vec2i getSize() = 0;
		//virtual void render(float) {};
		bool isFocus() { return m_IsFocus; }
		static bool IsInBox(::Vec2i, ::Vec2i, ::Vec2i);
	protected:
		Base();
		virtual ~Base() {};
		bool m_IsFocus;
	private:
	};
}