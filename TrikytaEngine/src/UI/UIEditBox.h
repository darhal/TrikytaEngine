#pragma once
#include "Base/UIEditBoxBase.h"
#include "core/Common/Color.h"
#include <string>
#include <SDL/SDL.h>

namespace UI {
	class Text;

	class EditBox : public EditBoxBase
	{
	public:
		static EditBox* createEditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color) {
			return new EditBox(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
		}

		EditBox(const std::string&, const std::string&, uint8, Vec2i, Color);
		void buildWidget();
		virtual void OnUIClick(Vec2i, bool) override;
		inline virtual Vec2i getPos() override;
		inline virtual Vec2i getSize() override;
		virtual void PorcessEvents(SDL_Event&) override;
		virtual void render(float dt) override;

		UI::Text* getText() {return m_InputText;}
	private:
		void UpdateText();
		SDL_Texture* widget_texture;
		UI::Text* m_InputText;
		SDL_Rect m_WidgetBounderies;
	};
}