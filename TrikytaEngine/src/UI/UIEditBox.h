#pragma once
#include <string>
#include "UIBase.h"
#include "core/Common/Color.h"

namespace UI {
	class Text;

	class EditBox : public Base
	{
	public:
		static EditBox* createEditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color) {
			return new EditBox(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
		}

		EditBox(const std::string&, const std::string&, uint8, Vec2i, Color);

		virtual void OnUIClick(Vec2i, bool) override;
		virtual void OnUIFocus(bool) override;
		inline virtual Vec2i getPos() override;
		inline virtual Vec2i getSize() override;
		virtual void PorcessEvents(SDL_Event&) override;

		UI::Text* getText() {return m_InputText;}

		void ActivateEditing(bool isActive);
	private:
		void UpdateText(std::string& p_Text);
		UI::Text* m_InputText;
		bool IsEditActive;
	};
}