#include "UIButton.hpp"
#include "misc/Font.h"
#include "UIManager.h"
#include "UI/UIText.h"
#include "core/Common/TrikytaEngine.h"

using namespace UI;

Button::Button(const std::string& p_text, Font* font, const Vec2i& p_pos, const Vec2i& p_size, const Color& text_color) 
{
	Manager::addElement(this, true);

	m_BtnText = Text::createText(p_text, font, p_pos, text_color, false);
	m_BtnText->setPosition(p_pos + (p_size) / 2 - m_BtnText->getSize() / 2);
	m_Pos = p_pos;
	m_Size = p_size;
	Button::buildWidget();
}

void Button::buildWidget()
{
	auto r = ENGINE->getRenderer();
	int onClickedEffectAmount = 10;
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
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

void Button::render(float dt)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);
	m_BtnText->render(dt);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void Button::OnUIClick(Vec2i pos, bool is_down)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, widget_texture);
	if (is_down) {
		int padding = 2;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 10, 206, 250, 220);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			SDL_SetRenderDrawColor(r, 0, 0, 0, sartingAlpha - i * 4);
		}
	}else {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 10, 250, 170, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
		}
	}
	SDL_SetRenderTarget(r, NULL);
}

void Button::OnMouseHover(Vec2i pos, bool isHover)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, widget_texture);
	if (isHover) {
		int padding = 1;
		auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
		SDL_RenderClear(r);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
		SDL_RenderFillRect(r, &widgetBounderies);
		int sartingAlpha = 200;
		SDL_SetRenderDrawColor(r, 10, 250, 170, 220);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
			SDL_SetRenderDrawColor(r, 0, 0, 0, sartingAlpha - i * 4);
		}
	}else {
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
	}
	SDL_SetRenderTarget(r, NULL);
}

void Button::PorcessEvents(SDL_Event& e)
{
	if (e.type == SDL_MOUSEMOTION) {
		Vec2i maxPos = getSize() + getPos();
		auto mc = Vec2i(e.motion.x, e.motion.y);
		if (!m_IsHover && IsInBox(mc, getPos(), maxPos)){
			m_IsHover = true;
			OnMouseHover(mc-getPos(), m_IsHover);
		}else if (m_IsHover && !IsInBox(mc, getPos(), maxPos)){
			m_IsHover = false;
			OnMouseHover(mc - getPos(), m_IsHover);
		}
	}
	Base::PorcessEvents(e);
}