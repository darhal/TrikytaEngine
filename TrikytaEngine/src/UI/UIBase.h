#pragma once
#include <map>
#include <vector>
#include "core/Common/defines.h"
#include <core/Common/Macros.hpp>

union SDL_Event;

namespace UI {

	union callbacksType
	{
		std::function<void(const Vec2i&, bool)> mouseCallBack;
		callbacksType() {}
		callbacksType(const callbacksType& o): mouseCallBack(o.mouseCallBack){}
		~callbacksType() {};
	};

	class Base
	{
		REGISTER_EVENT(ON_UI_CLICK, void(const Vec2i&, bool));
		REGISTER_EVENT(ON_UI_FOCUS, void(bool, const Vec2i&));
		REGISTER_EVENT(ON_UI_HOVER, void(const Vec2i&, bool));
	public:
		virtual void PorcessEvents(SDL_Event&);
		virtual void OnUIClick(const ::Vec2i&, bool) {};
		virtual void OnUIFocus(bool, const ::Vec2i&) {};
		virtual void OnUIMouseHover(const ::Vec2i&, bool) {};
		virtual Vec2i getPos() = 0;
		virtual Vec2i getSize() = 0;
		virtual void setPos(const Vec2i&) {};
		virtual void render(float) {};
		virtual void setVisible(bool p_Visible);
		virtual bool isVisible();
		bool isFocus() { return m_IsFocus; }
		bool isHover() { return m_IsHover; }
		void Enable(bool);
		static bool IsInBox(const ::Vec2i&, const ::Vec2i&, const ::Vec2i&);
	protected:
		Base();
		virtual ~Base() {};
		bool m_IsFocus;
		bool m_IsVsisible;
		bool m_IsHover;
	private:
		friend class Button;
		friend class EditBox;
		friend class Text;
	};
}