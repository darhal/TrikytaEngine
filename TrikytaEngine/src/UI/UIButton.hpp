#pragma once
#include "UIBase.h"
#include <string>
#include <SDL/SDL.h>
#include "core/Common/Vec2.h"
#include "core/Common/Color.h"
#include "core/Common/Macros.hpp"

class Font;

namespace UI
{
	class Button : public Base {
	public:
		static Button* createButton(const std::string& text, Font* font, const Vec2i& pos, const Vec2i& size, const Color& color)
		{
			return new Button(text, font, pos, size, color);;
		}

		Button(const std::string&, Font*, const Vec2i&, const Vec2i&, const Color&);
		virtual void OnUIClick(const Vec2i&, bool) override;
		virtual void OnUIFocus(bool, const Vec2i&) {};
		virtual Vec2i getPos() { return m_Pos; };
		virtual Vec2i getSize() { return m_Size; };
		virtual void render(float) override;
		virtual void PorcessEvents(SDL_Event& e) override;
		virtual void OnUIMouseHover(const Vec2i&, bool) override;

		void setPos(const Vec2i&) override;
	private:
		class Text* m_BtnText = nullptr;
		Vec2i m_Pos;
		Vec2i m_Size;
		SDL_Rect m_WidgetBounderies;
		SDL_Texture* widget_texture;
		bool m_IsHover;
	private:
		void buildWidget();
	};
}