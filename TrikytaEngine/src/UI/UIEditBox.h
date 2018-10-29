#pragma once
#include "Base/UIEditBoxBase.h"
#include "core/Common/Color.h"
#include <string>

//TODO: Find a solution when you switch directly from an editbox to another (due to events treated one after the other)

namespace UI {
	class Text;

	class EditBox : public EditBoxBase
	{
	public:
		static EditBox* createEditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color) {
			return new EditBox(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
		}

		EditBox(const std::string&, const std::string&, uint8, Vec2i, Color);

		virtual void OnUIClick(Vec2i, bool) override;
		inline virtual Vec2i getPos() override;
		inline virtual Vec2i getSize() override;
		virtual void PorcessEvents(SDL_Event&) override;
		virtual void render(float dt) override;

		UI::Text* getText() {return m_InputText;}
	private:
		void UpdateText();
		UI::Text* m_InputText;
	};
}