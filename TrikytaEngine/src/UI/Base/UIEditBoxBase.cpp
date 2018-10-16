#include "core/InputManager/InputManager.h"
#include "UIEditBoxBase.h"
#include "UI/UIManager.h"

using namespace UI;
//TODO: To fix the problem when switch editbox immediatly to another without click nowhere
void EditBoxBase::OnUIFocus(bool isFocus)
{
	if (isFocus) {
		InputManager::getInputManager()->setCurosrPosition(getPos(), getSize());
		Manager::addElement(this, isFocus);
	}
	else {
		Manager::removeElement(this, isFocus);
	}
	IsEditActive = isFocus;
	InputManager::getInputManager()->ActivateInput(IsEditActive);
}