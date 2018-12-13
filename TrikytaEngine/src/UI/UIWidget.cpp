#include "UIWidget.hpp"
#include "UIText.h"
#include "core/Common/TrikytaEngine.h"
#include "UIManager.h"

using namespace UI;

Widget::Widget(const std::string& title, const Vec2i& pos, const Vec2i& size): 
	m_Pos(pos), m_Size(size), m_Title(title)
{
	Manager::addElement(this, true);
	auto r = ENGINE->getRenderer();
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);
}


Widget::Widget(const Vec2i& pos, const Vec2i& size): 
	m_Pos(pos), m_Size(size), m_Title("")
{
	Manager::addElement(this, true);
	auto r = ENGINE->getRenderer();
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
	widget_texture = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_WidgetBounderies.w, m_WidgetBounderies.h);
}

void Widget::buildWidget(Font* font, const Color& windowColor, const Color& contourColor, const Color& topColor, const Color& titleColor)
{
	auto r = ENGINE->getRenderer();
	int padding = 1;
	auto widgetBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, m_Size.y - padding * 2 };
	SDL_SetRenderTarget(r, widget_texture);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
	SDL_RenderFillRect(r, &widgetBounderies);
	SDL_SetRenderDrawColor(r, topColor.r, topColor.g, topColor.b, topColor.a);
	m_TitleText = Text::createText(m_Title, font, Vec2i(0,0), titleColor, false);
	auto titleBounderies = SDL_Rect{ padding, padding, m_Size.x - padding * 2, int(m_TitleText->getSize().y*1.2f) - padding * 2 };
	m_TitleText->setPosition((Vec2i(titleBounderies.w, titleBounderies.h) - m_TitleText->getSize())/2);
	SDL_RenderFillRect(r, &titleBounderies);
	m_TitleText->render(0.f);
	SDL_SetRenderDrawColor(r, contourColor.r, contourColor.g, contourColor.b, contourColor.a);
	for (int i = padding; i >= 0; i--) {
		widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.x - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
		SDL_RenderDrawRect(r, &widgetBounderies);
	}
	SDL_SetRenderTarget(r, NULL);
}

void Widget::Configure(const WidgetParam& config)
{
	buildWidget(config.font, config.windowColor, config.contourColor, config.topColor, config.titleColor);
}

void Widget::PorcessEvents(SDL_Event& e)
{
	Base::PorcessEvents(e);
	ManageWidgetMovment(e);
}

void Widget::ManageWidgetMovment(SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN && Base::IsInBox(Vec2i(e.button.x, e.button.y), Vec2i(m_WidgetBounderies.x, m_WidgetBounderies.y),
		Vec2i(m_WidgetBounderies.w + m_WidgetBounderies.x, m_WidgetBounderies.y + int(m_TitleText->getSize().y*1.2f))))
	{
		m_IsClicked = true;
		m_ClickedPosition = Vec2i(m_WidgetBounderies.x, m_WidgetBounderies.y) - Vec2i(e.button.x, e.button.y);
	}
	else if (e.type == SDL_MOUSEBUTTONUP && m_IsClicked) {
		m_IsClicked = false;
	}
	else if (e.type == SDL_MOUSEMOTION && m_IsClicked) {
		setPos(Vec2i(e.motion.x, e.motion.y) + m_ClickedPosition);
	}
}

void Widget::render(float dt)
{
	auto r = ENGINE->getRenderer();
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(widget_texture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, widget_texture, NULL, &m_WidgetBounderies);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
	for (Base* itr : m_Elements) {
		itr->render(dt);
	}
}

void Widget::OnUIClick(const ::Vec2i& pos, bool is_down)
{

}

void Widget::setPos(const Vec2i& pos)
{
	Vec2i posDiff = Vec2i(m_WidgetBounderies.x-pos.x, m_WidgetBounderies.y - pos.y);
	m_WidgetBounderies.x = pos.x;
	m_WidgetBounderies.y = pos.y;
	for (Base* uiElemnt : m_Elements){
		Vec2i uipos = uiElemnt->getPos();
		uiElemnt->setPos(uipos-posDiff);
	}
}

void Widget::AddElement(Base* uiElement)
{
	Manager::removeElement(uiElement, true);
	m_Elements.emplace_back(uiElement);
	Vec2i pos = uiElement->getPos();
	uiElement->setPos(pos + Vec2i(m_WidgetBounderies.x, m_WidgetBounderies.y));
}