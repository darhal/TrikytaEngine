#include "UIBase.h"
#include "UIManager.h"
#include <SDL/SDL.h>
#include "core/Common/defines.h"

using namespace UI;

Base::Base()
{
	Manager::addElement(this);
	m_IsVsisible = true;
}

bool Base::IsInBox(const Vec2i& mouseClick, const Vec2i& minPos, const Vec2i& maxPos)
{
	return (mouseClick.x >= minPos.x && mouseClick.y >= minPos.y) && (mouseClick.x <= maxPos.x && mouseClick.y <= maxPos.y);
}

void Base::PorcessEvents(SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		Vec2i maxPos = getSize() + getPos();
		auto mc = Vec2i(e.button.x, e.button.y);
		if (IsInBox(mc, getPos(), maxPos))
		{
			if (!m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus, mc);
				TRIGGER_EVENT(ON_UI_FOCUS, !m_IsFocus, mc);
			}
			m_IsFocus = true;
			OnUIClick(mc - getPos(), e.type == SDL_MOUSEBUTTONDOWN ? true : false);
			TRIGGER_EVENT(ON_UI_CLICK, mc - getPos(), e.type == SDL_MOUSEBUTTONDOWN ? true : false);
		}else{
			if (m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus, mc);
				TRIGGER_EVENT(ON_UI_FOCUS, !m_IsFocus, mc);
			}
			m_IsFocus = false;
		}
	}else if (e.type == SDL_MOUSEMOTION) {
		Vec2i maxPos = getSize() + getPos();
		auto mc = Vec2i(e.motion.x, e.motion.y);
		if (!m_IsHover && IsInBox(mc, getPos(), maxPos)) {
			m_IsHover = true;
			TRIGGER_EVENT(ON_UI_HOVER, mc - getPos(), m_IsHover);
			OnUIMouseHover(mc - getPos(), m_IsHover);
		}
		else if (m_IsHover && !IsInBox(mc, getPos(), maxPos)) {
			m_IsHover = false;
			TRIGGER_EVENT(ON_UI_HOVER, mc - getPos(), m_IsHover);
			OnUIMouseHover(mc - getPos(), m_IsHover);
		}
	}
}

void Base::setVisible(bool p_Visible) 
{ 
	if (p_Visible) {
		Manager::addElement(this, true); //Activate Render
		Manager::addElement(this, false); //Activate Events
	}else {
		Manager::removeElement(this, true); //Remove Render
		Manager::removeElement(this, false); //Remove Events
	}
	m_IsVsisible = p_Visible; 
};

bool Base::isVisible() 
{ 
	return m_IsVsisible; 
};