#pragma once
#include <map>
#include <vector>
#include "core/Common/defines.h"

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
	public:
		virtual void PorcessEvents(SDL_Event&);
		virtual void OnUIClick(const ::Vec2i&, bool) = 0;
		virtual void OnUIFocus(bool, const ::Vec2i&) = 0;
		virtual Vec2i getPos() = 0;
		virtual Vec2i getSize() = 0;
		virtual void setPos(const Vec2i&) {};
		virtual void render(float) {};
		virtual void setVisible(bool p_Visible);
		virtual bool isVisible();
		bool isFocus() { return m_IsFocus; }
		static bool IsInBox(const ::Vec2i&, const ::Vec2i&, const ::Vec2i&);
	protected:
		Base();
		virtual ~Base() {};
		bool m_IsFocus;
		bool m_IsVsisible;
	private:
		friend class Button;
		friend class EditBox;
		friend class Text;
	};
}