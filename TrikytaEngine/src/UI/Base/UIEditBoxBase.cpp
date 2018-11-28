#include "core/InputManager/InputManager.h"
#include "UIEditBoxBase.h"
#include "UI/UIManager.h"
#include "core/Common/defines.h"
#include "UI/UIText.h"

using namespace UI;

void EditBoxBase::OnUIFocus(bool isFocus, Vec2i pMouseClick)
{
	IsEditActive = isFocus;
	if (isFocus) {
		OnToggleEdit(true);
		InputManager::getInputManager()->setCurosrPosition(getTextPos(), getTextSize());
		//Manager::addElement(this, true);
	}else{
		//Manager::removeElement(this, true);
		OnToggleEdit(false);
		for (const auto& obj : Manager::getUIContainer()) {
			Vec2i maxPos = obj->getSize() + obj->getPos();
			Vec2i minPos = obj->getPos();
			if (Console::getConsole()->IsActive() && dynamic_cast<EditBoxBase*>(obj)) {
				auto console_rect = Console::getConsole()->getConsoleBoundries();
				maxPos = Vec2i(console_rect.x + console_rect.w, console_rect.y + console_rect.h);
				minPos = Vec2i(console_rect.x, console_rect.y);
			}
			if (IsInBox(pMouseClick, minPos, maxPos) && dynamic_cast<EditBoxBase*>(obj) != nullptr) {
				//LogConsole(LogError, "!!! >> Forcing the activate << !!!");
				InputManager::getInputManager()->ActivateInput(true);
				return;
			}
		}
	}
	InputManager::getInputManager()->ActivateInput(IsEditActive);
}

Vec2i EditBoxBase::getTextSize() {
	return m_InputText->getSize();
};
Vec2i EditBoxBase::getTextPos() {
	return m_InputText->getPosition();
};