#pragma once
#include "Base/UIEditBoxBase.h"
#include "core/Common/Color.h"
#include "core/Common/Macros.hpp"
#include <string>
#include <SDL/SDL.h>

namespace UI {
	class Text;

	class EditBox : public EditBoxBase
	{
	public:
		static EditBox* createEditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, const Vec2i& p_Pos, const Vec2i& p_Size,const Color& p_Color) {
			return new EditBox(p_Text, p_Font, p_TextSize, p_Pos, p_Size, p_Color);
		}

		EditBox(const std::string&, const std::string&, uint8, const Vec2i&, const Vec2i&,const Color&);
		void buildWidget();
		virtual void OnUIClick(const Vec2i&, bool) override;
		virtual void OnToggleEdit(bool) override;
		inline virtual Vec2i getPos() override;
		inline virtual Vec2i getSize() override;
		virtual void PorcessEvents(SDL_Event&) override;
		virtual void render(float dt) override;
		REGISTER_EVENT(ON_EDITBOX_CHANGE, void(const char*));
		REGISTER_EVENT(ON_EDITBOX_FOCUS, void(bool));
	private:
		Vec2i m_Size, m_Pos;
		void UpdateText();
		SDL_Texture* widget_texture;
		SDL_Rect m_WidgetBounderies;
		SDL_Rect m_WidgetSourceDraw;
		int m_onClickedEffectAmount;
	};
}