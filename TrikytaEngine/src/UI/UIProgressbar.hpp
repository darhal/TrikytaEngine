#pragma once
#include "UIBase.h"
#include "core/Common/Vec2.h"
#include <SDL/SDL.h>
#include <core/Common/Color.h>

class Font;

namespace UI
{
	class Text;

	class Progressbar : public Base
	{
	public:
		Progressbar(const Vec2i&, const Vec2i&, const Color&, const Color&, const std::string& = "", const Color& = { 255, 255, 255, 255 }, Font* = nullptr);
		virtual void setPos(const Vec2i& pos) { m_Pos = pos;  m_WidgetBounderies.x = pos.x; m_WidgetBounderies.y = pos.y; };
		virtual void render(float) override;		
		Vec2i getPos() { return m_Pos; };
		Vec2i getSize() { return m_Size; };
		~Progressbar();
		void setProgress(int);
		void setProgress(int, Color&&, Color&&, int);
		int getProgress() { return m_Progress; }
	protected:

	private:
		void buildWidget();
		SDL_Rect m_WidgetBounderies;
		SDL_Texture* widget_texture;
		Color widgetColor;
		Color progressbarColor;
		//SDL_Texture* progressbar_texture;
		Vec2i m_Pos;
		Vec2i m_Size;
		Text* m_ProgressbarText;
		int m_Progress;
	};
}