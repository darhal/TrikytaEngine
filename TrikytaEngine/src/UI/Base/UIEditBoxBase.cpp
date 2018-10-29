#include "core/InputManager/InputManager.h"
#include "UIEditBoxBase.h"
#include "UI/UIManager.h"
#include "core/Common/defines.h"

using namespace UI;
//TODO: To fix the problem when switch editbox immediatly to another without click nowhere
void EditBoxBase::OnUIFocus(bool isFocus, Vec2i pMouseClick)
{
	IsEditActive = isFocus;
	bool desactivateOnLeave = true;
	if (isFocus) {
		InputManager::getInputManager()->setCurosrPosition(getPos(), getSize());
		Manager::addElement(this, true);
	} else {
		Manager::removeElement(this, true);
		for (const auto& obj : Manager::getUIContainer()) {
			Vec2i maxPos = obj->getSize() + obj->getPos();
			if (IsInBox(pMouseClick, obj->getPos(), maxPos) && dynamic_cast<EditBoxBase*>(obj) != nullptr) {
				desactivateOnLeave = false;
				break;
			}
		}
	}
	if (desactivateOnLeave) {
		InputManager::getInputManager()->ActivateInput(IsEditActive);
	}
}