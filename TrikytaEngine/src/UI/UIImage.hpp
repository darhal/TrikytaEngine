#pragma once
#include <string>
#include "UIBase.h"
#include <SDL/SDL.h>
#include <core/Common/Vec2.h>
#include <core/Common/Color.h>

namespace UI
{
	class Image : public Base 
	{
	public:
		Image(const std::string&, const Vec2i&, const Vec2i& = Vec2i(0, 0));

		/*virtual void OnUIClick(const ::Vec2i&, bool);
		virtual void OnUIFocus(bool, const ::Vec2i&);
		virtual void OnUIMouseHover(const ::Vec2i&, bool);*/
		virtual Vec2i getPos() { return m_Pos; };
		virtual Vec2i getSize() { return m_Size; };
		void render(float) override;
		virtual void setPos(const Vec2i& pos) { m_Pos = pos; m_WidgetBounderies.x = pos.x; m_WidgetBounderies.y = pos.y; };
		void buildWidget(const std::string&);
		void ChangeTexture(const std::string&);
		void SetColor(const Color&);
	protected:

	private:
		Vec2i m_Pos, m_Size;
		SDL_Texture* m_Texture;
		SDL_Rect m_WidgetBounderies;
	};
}