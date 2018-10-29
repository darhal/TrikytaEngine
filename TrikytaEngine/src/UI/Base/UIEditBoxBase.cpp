#include "core/InputManager/InputManager.h"
#include "UIEditBoxBase.h"
#include "UI/UIManager.h"
#include "core/Common/defines.h"

using namespace UI;

void EditBoxBase::OnUIFocus(bool isFocus, Vec2i pMouseClick)
{
	IsEditActive = isFocus;
	if (isFocus) {
		InputManager::getInputManager()->setCurosrPosition(getPos(), getSize());
		Manager::addElement(this, true);
	}else{
		Manager::removeElement(this, true);
		//LogConsole(LogWarning, "Size of container : %d", Manager::getUIContainer().size());
		for (const auto& obj : Manager::getUIContainer()) {
			Vec2i maxPos = obj->getSize() + obj->getPos();
			Vec2i minPos = obj->getPos();
			if (Console::getConsole()->IsActive() && dynamic_cast<EditBoxBase*>(obj)) {
				auto console_rect = Console::getConsole()->getConsoleBoundries();
				maxPos = Vec2i(console_rect.x + console_rect.w, console_rect.y + console_rect.h);
				minPos = Vec2i(console_rect.x, console_rect.y);
			}
			//LogConsole(LogError, "IsInBox({%d, %d}, {%d, %d}, {%d, %d})", pMouseClick.x, pMouseClick.y, minPos.x, minPos.y, maxPos.x, maxPos.y);
			if (IsInBox(pMouseClick, minPos, maxPos) && dynamic_cast<EditBoxBase*>(obj) != nullptr) {
				//LogConsole(LogError, "!!! >> Forcing the activate << !!!");
				InputManager::getInputManager()->ActivateInput(true);
				return;
			}
		}
	}
	InputManager::getInputManager()->ActivateInput(IsEditActive);
}