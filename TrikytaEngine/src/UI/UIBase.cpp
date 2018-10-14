#include "UIBase.h"
#include "UIManager.h"
#include <SDL/SDL.h>
#include "core/Common/defines.h"

using namespace UI;

Base::Base()
{
	Manager::addElement(this);
}

bool Base::IsInBox(Vec2i mouseClick, Vec2i minPos, Vec2i maxPos)
{
	return (mouseClick.x >= minPos.x && mouseClick.y >= minPos.y) && (mouseClick.x <= maxPos.x && mouseClick.y <= maxPos.y);
}

void Base::PorcessEvents(SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		Vec2i maxPos = getSize() + getPos();
		auto mc = Vec2i(e.button.x, e.button.y);
		//LogConsole(LogInfo, "Mouse (X=%d, Y=%d) | Widget (X=%d, Y=%d, W=%d, H=%d)", e.button.x, e.button.y, getPos().x, getPos().y, maxPos.x, maxPos.y);
		if (IsInBox(mc, getPos(), maxPos))
		{
			if (!m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus);
			}
			m_IsFocus = true;
			OnUIClick(mc - getPos(), e.type == SDL_MOUSEBUTTONDOWN ? true : false);
		}else{
			if (m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus);
			}
			m_IsFocus = false;
		}
	}
}