#include "UIEditBox.h"
#include <SDL/SDL.h>
#include "core/Common/Color.h"
#include "UIText.h"
#include "UIManager.h"
#include "Events/UIEventHandler.h"
#include "core/InputManager/InputManager.h"

using namespace UI;

EditBox::EditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color)
{
	m_InputText = Text::createText(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
}

Vec2i EditBox::getPos()
{ 
	return m_InputText->getPosition(); 
};

Vec2i EditBox::getSize()
{ 
	return m_InputText->getSize(); 
};

void EditBox::PorcessEvents(SDL_Event& e)
{
	Base::PorcessEvents(e);
	if (IsEditActive) {
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && m_InputText->m_Text.length() > 0)//Handle backspace
			{
				m_InputText->m_Text.pop_back();
				EditBox::UpdateText();
			}
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {//Handle copy
				SDL_SetClipboardText(m_InputText->m_Text.c_str());
			}
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { //Handle paste
				m_InputText->m_Text = SDL_GetClipboardText();
				EditBox::UpdateText();
			}
		}
		else if (e.type == SDL_TEXTINPUT) { //Special text input event
			  //Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				//Append character
				m_InputText->m_Text += e.text.text;
				m_InputText->updateTextHelper();
				EditBox::UpdateText();
			}
		}
		else if (e.type == SDL_TEXTEDITING) {
			/*composition = event.edit.text;
			cursor = event.edit.start;
			selection_len = event.edit.length;*/
		}
	}
}

void EditBox::UpdateText()
{
	m_InputText->updateTextHelper();
	InputManager::getInputManager()->setCurosrPosition(getPos(), getSize());
}

void EditBox::OnUIClick(Vec2i pos, bool isDown) 
{

}

void EditBox::render(float dt)
{
	InputManager::getInputManager()->DrawCursor(dt);
}

/*void EditBox::OnUIFocus(bool isFocus)
{
	if (isFocus) {
		InputManager::getInputManager()->setCurosrPosition(getPos(), getSize());
		UI::Manager::addElement(this, isFocus);
	}else {
		UI::Manager::removeElement(this, isFocus);
	}
	IsEditActive = isFocus;
	InputManager::getInputManager()->ActivateInput(IsEditActive);
};*/
