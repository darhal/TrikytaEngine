#pragma once
#include "BaseMenu.hpp"
#include <core/Common/defines.h>
#include <vector>

class InGameMenu : public BaseMenu
{
public:
	InGameMenu(class GUIManager*, class GameManager*);
	InGameMenu():BaseMenu(nullptr){};
	virtual ~InGameMenu();
	void BuildInGameMenu();
	void OnRootButtonClick(bool);
	void ShowMenu(bool);
	class GameManager* m_GameManager;
private:
	UI::Widget* m_Widget;
	std::vector<UI::Base*> m_InGameMenu;
	class SoundEffect* m_ClickEffect;
	std::string m_CurrentButton;
};