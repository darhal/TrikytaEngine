#include "UIWidget.hpp"
#include "UIText.h"
#include "core/Common/TrikytaEngine.h"
#include "UIManager.h"

using namespace UI;

Widget::Widget(const std::string& title, const Vec2i& pos, const Vec2i& size, const Color& color): m_Pos(pos), m_Size(size), m_Color(color), m_Title(title)
{
	Widget::buildWidget();
}


Widget::Widget(const Vec2i& pos, const Vec2i& size, const Color& color): m_Pos(pos), m_Size(size), m_Color(color), m_Title("")
{
	Widget::buildWidget();
}

void Widget::buildWidget()
{
	Manager::addElement(this, true);
	auto r = ENGINE->getRenderer();
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);
	int padding = 1;
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderFillRect(r, &widgetBounderies);

	SDL_SetRenderDrawColor(r, 255-m_Color.r, 255-m_Color.g, 255-m_Color.b, m_Color.a);
	auto title = Text::createText(m_Title, Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 16), Vec2i(0,0), m_Color, false);
	auto titleBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2,  title->getSize().y*2 - padding * 2 };
	title->setPosition((Vec2i(titleBounderies.w, titleBounderies.h) - title->getSize())/2);
	SDL_RenderFillRect(r, &titleBounderies);
	title->render(0.f);
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	SDL_SetRenderTarget(r, NULL);
}

void Widget::render(float /*dt*/)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

void Widget::OnUIClick(const ::Vec2i& pos, bool is_down)
{

}