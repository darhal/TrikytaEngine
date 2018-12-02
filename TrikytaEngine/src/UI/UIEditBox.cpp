#include "UIEditBox.h"
#include <SDL/SDL.h>
#include "core/Common/Color.h"
#include "UIText.h"
#include "UIManager.h"
#include "Events/UIEventHandler.h"
#include "core/InputManager/InputManager.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Common/Macros.hpp"

using namespace UI;

EditBox::EditBox(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, const Vec2i& p_Pos, const Vec2i& p_Size,const Color& p_Color)
{
	Manager::addElement(this, true);

	m_InputText = Text::createText(p_Text, p_Font, p_TextSize, p_Pos, p_Color, Font::Style::NORMAL, false);
	m_InputText->setPosition(p_Pos + (Vec2i(10, p_Size.y) / 2) - Vec2i(0, m_InputText->getSize().y) / 2);
	m_Pos = p_Pos;
	m_Size = p_Size;
	EditBox::buildWidget();
}

void EditBox::buildWidget()
{
	auto r = ENGINE->getRenderer();
	int onClickedEffectAmount = 10;
	m_WidgetBounderies = SDL_Rect{m_Pos.x, m_Pos.y, m_Size.x, m_Size.y};
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);
	int padding = 1;
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	SDL_SetRenderTarget(r, NULL);
}

Vec2i EditBox::getPos()
{
	return m_Pos;
};

Vec2i EditBox::getSize()
{
	return m_Size;
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
		}else if (e.type == SDL_TEXTINPUT) { //Special text input event
			  //Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				//Append character
				m_InputText->m_Text += e.text.text;
				TRIGGER_EVENT(ON_EDITBOX_CHANGE, e.text.text);
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
	InputManager::getInputManager()->setCurosrPosition(getTextPos(), getTextSize());
}

void EditBox::OnUIClick(const Vec2i& pos, bool isDown)
{

}

void EditBox::render(float dt)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);

	
	m_InputText->render(dt);
	if (IsEditActive) { 
		int editPadding = 2;
		auto widgetBounderies = SDL_Rect{ m_Pos.x-editPadding, m_Pos.y - editPadding, m_Size.x + editPadding * 2, m_Size.y + editPadding * 2 };
		Color color = Color(10, 206, 250, 220);
		SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
		for (int i = editPadding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x + 1, widgetBounderies.x + 1, widgetBounderies.w - 2, widgetBounderies.h - 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			color.a -= i * 17;
			color.b -= i * 17;
			SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
		}
		InputManager::getInputManager()->DrawCursor(dt); 
	}
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void EditBox::OnToggleEdit(bool isEdit)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, widget_texture);
	if (isEdit) {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding*2, m_Size.y - padding*2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 10, 206, 250, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			SDL_SetRenderDrawColor(r, 10, 206, 250, sartingAlpha - i*4);
		}
		TRIGGER_EVENT(ON_EDITBOX_FOCUS, isEdit);
	}else{
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
		}
		TRIGGER_EVENT(ON_EDITBOX_FOCUS, isEdit);
	}
	SDL_SetRenderTarget(r, NULL);
};

/*
	m_WidgetBounderies = SDL_Rect{m_Pos.x, m_Pos.y, m_Size.x, m_Size.y};
	m_WidgetSourceDraw = SDL_Rect{ m_onClickedEffectAmount, m_onClickedEffectAmount, m_WidgetBounderies.w, m_WidgetBounderies.h};
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w+m_onClickedEffectAmount, m_WidgetBounderies.h+m_onClickedEffectAmount);
	int sartingAlpha = 200;
	auto widgetBounderies = SDL_Rect{ 0, 0, m_Size.x + m_onClickedEffectAmount, m_Size.y + m_onClickedEffectAmount };
	SDL_SetRenderDrawColor(r, 10, 206, 250, sartingAlpha);
	for (int i = m_onClickedEffectAmount; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x + 1, widgetBounderies.x + 1, widgetBounderies.w - 2, widgetBounderies.h - 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
		SDL_SetRenderDrawColor(r, 10, 206, 250, sartingAlpha + i * i);
	}
	int padding = 2;
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x + 1, widgetBounderies.x + 1, widgetBounderies.w - 2, widgetBounderies.h - 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 165);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderTarget(r, NULL);
*/