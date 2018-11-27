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
	Manager::addElement(this, true);
	m_InputText = Text::createText(p_Text, p_Font, p_TextSize, p_Pos, p_Color, Font::Style::NORMAL, false);
	EditBox::buildWidget();
}

void EditBox::buildWidget()
{
	auto r = ENGINE->getRenderer();
	
	auto widgetBounderies = SDL_Rect{ 1, 1, m_InputText->getSize().x, m_InputText->getSize().y };
	m_WidgetBounderies = SDL_Rect{ getPos().x, getPos().y, widgetBounderies.w + 2, widgetBounderies.h + 2 };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);

	SDL_SetRenderTarget(r, widget_texture);
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 175);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_RenderDrawRect(r, &widgetBounderies);
	widgetBounderies = SDL_Rect{ 0, 0, m_WidgetBounderies.w, m_WidgetBounderies.h };
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 255);
	SDL_RenderDrawRect(r, &widgetBounderies);
	SDL_SetRenderTarget(r, NULL);
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
	auto r = ENGINE->getRenderer();

	/*auto widgetBounderies = SDL_Rect{ m_InputText->getPosition().x, m_InputText->getPosition().y, m_InputText->getSize().x, m_InputText->getSize().y };
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 175);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_RenderDrawRect(r, &widgetBounderies);
	SDL_RenderDrawRect(r, &m_WidgetBounderies);
	SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);*/

	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(ENGINE->getRenderer(), widget_texture, NULL, &m_WidgetBounderies);
	m_InputText->render(dt);
	if (IsEditActive) { InputManager::getInputManager()->DrawCursor(dt); }
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
