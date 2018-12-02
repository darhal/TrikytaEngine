#pragma once
#include "UIBase.h"
#include "core/Common/Vec2.h"
#include "core/Common/Color.h"

namespace UI
{
	class Widget : public Base
	{
	public:
		Widget(const std::string&, const Vec2i&, const Vec2i&, const Color&);
		Widget(const Vec2i&, const Vec2i&, const Color&);
		virtual void render(float) override;
		virtual void OnUIClick(const ::Vec2i&, bool);
		virtual void OnUIFocus(bool, const ::Vec2i&) {};
		virtual Vec2i getPos() { return m_Pos; };
		virtual Vec2i getSize() { return m_Size; };
	private:
		void buildWidget();
		SDL_Texture* widget_texture;
		SDL_Rect m_WidgetBounderies;
		Vec2i m_Pos;
		Vec2i m_Size;
		Color m_Color;
		std::string m_Title;
	};
}