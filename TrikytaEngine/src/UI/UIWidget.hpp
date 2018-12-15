#pragma once
#include "UIBase.h"
#include "core/Common/Vec2.h"
#include "core/Common/Color.h"
#include <misc/Font.h>

namespace UI
{
	struct WidgetParam {
		WidgetParam(Font* font=Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 18), const Color& windowColor = { 124,124,124,255 }, const Color& contourColor = { 0, 0, 0, 255 }, const Color& topColor= { 66, 206, 244, 255 }, const Color& titleColor={0, 0, 0, 255}):
			windowColor(windowColor), contourColor(contourColor), topColor(topColor), titleColor(titleColor), font(font)
		{}
		Color windowColor;
		Color contourColor;
		Color topColor;
		Color titleColor;
		Font* font;
	};

	class Widget : public Base
	{
	public:
		Widget(const std::string&, const Vec2i&, const Vec2i&);
		Widget(const Vec2i&, const Vec2i&);
		virtual void render(float) override;
		virtual void OnUIClick(const ::Vec2i&, bool);
		virtual void OnUIFocus(bool, const ::Vec2i&) {};
		virtual Vec2i getPos() { return m_Pos; };
		virtual Vec2i getSize() { return m_Size; };
		virtual void PorcessEvents(SDL_Event&) override;
		virtual void setVisible(bool p_Visible) override;
		void setPos(const Vec2i&) override;
		void Configure(const WidgetParam&);

		void AddElement(Base* uiElement);
		inline const std::vector<Base*>& GetElements(){return m_Elements; };
	private:
		void buildWidget(Font*, const Color& = Color{124,124,124,255}, const Color& = {0, 0, 0, 255}, const Color& = { 66, 206, 244, 255 }, const Color& = {0, 0, 0, 255});
		void ManageWidgetMovment(SDL_Event&);
		SDL_Texture* widget_texture;
		SDL_Rect m_WidgetBounderies;
		Vec2i m_Pos;
		Vec2i m_Size;
		Vec2i m_ClickedPosition;
		bool m_IsClicked;
		std::string m_Title;
		class Text* m_TitleText;
		std::vector<Base*> m_Elements;
	};
}
