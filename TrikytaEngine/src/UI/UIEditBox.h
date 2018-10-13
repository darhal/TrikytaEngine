#pragma once
#include <string>
#include "core/Drawable/Drawable.h"
#include "core/Common/Color.h"

namespace UI {
	class Text;

	class EditBox
	{
	public:
		static EditBox* createEditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color) {
			return new EditBox(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
		}
		EditBox(const std::string&, const std::string&, uint8, Vec2i, Color);
		void PorcessInput(union SDL_Event&);
		UI::Text* getText() {return m_InputText;}
	private:
		void UpdateText(std::string& p_Text);
		UI::Text* m_InputText;
	};
}