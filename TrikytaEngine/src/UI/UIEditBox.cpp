#include "UIEditBox.h"
#include <SDL/SDL.h>
#include "core/Common/Color.h"
#include "UIText.h"
#include "UIManager.h"
#include "Events/UIEventHandler.h"
#include "core/InputManager/InputManager.h"
#include "core/Common/TrikytaEngine.h"

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
	SDL_Rect m_WidgetBounderies = SDL_Rect{ m_InputText->getPosition().x, m_InputText->getPosition().y, m_InputText->getSize().x, m_InputText->getSize().y};
	SDL_Rect m_WidgetLineBounderies = SDL_Rect{ m_InputText->getPosition().x-1, m_InputText->getPosition().y-1, m_InputText->getSize().x+2, m_InputText->getSize().y+2 };
	SDL_SetRenderDrawBlendMode(ENGINE->getRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 200);
	SDL_RenderFillRect(ENGINE->getRenderer(), &m_WidgetBounderies);
	SDL_RenderDrawRect(ENGINE->getRenderer(), &m_WidgetBounderies);
	SDL_RenderDrawRect(ENGINE->getRenderer(), &m_WidgetLineBounderies);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
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
