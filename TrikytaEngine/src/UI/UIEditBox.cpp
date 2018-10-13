#include "UIEditBox.h"
#include <SDL/SDL.h>
#include "core/Common/Color.h"
#include "UIText.h"

using namespace UI;

EditBox::EditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color)
{
	m_InputText = Text::createText(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
	//SDL_SetTextInputRect(rect);
}

void EditBox::PorcessInput(SDL_Event& e)
{
	if(e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_BACKSPACE && m_InputText->m_Text.length() > 0)//Handle backspace
		{
			LogConsole(LogWarning, "Handle backspace");
			m_InputText->m_Text.pop_back();
			m_InputText->updateTextHelper();
		} else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL){//Handle copy
			SDL_SetClipboardText(m_InputText->m_Text.c_str());
		} else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { //Handle paste
			m_InputText->m_Text = SDL_GetClipboardText();
			m_InputText->updateTextHelper();
		} 
	}else if (e.type == SDL_TEXTINPUT) { //Special text input event
		   //Not copy or pasting
		if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
		{
			//Append character
			m_InputText->m_Text += e.text.text;
			m_InputText->updateTextHelper();
		}
	}else if (e.type == SDL_TEXTEDITING) {
		/*composition = event.edit.text;
		cursor = event.edit.start;
		selection_len = event.edit.length;*/
	}
}

void EditBox::UpdateText(std::string& p_Text)
{
	m_InputText->updateText(p_Text);
}