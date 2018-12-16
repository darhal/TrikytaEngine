#pragma once
#include <vector>
#include <core/Common/defines.h>
#include <core/Common/Vec2.h>
#include <SDL/SDL.h>
#include "BaseMenu.hpp"

class MainMenu : public BaseMenu
{
public:
	MainMenu(class GUIManager*);
	virtual ~MainMenu();
	void BuildMainMenu();
	void Refresh(float dt);
	void OnMouseMove(const Vec2i&);
	void OnRootButtonClick(bool);
protected:

private:
	std::vector<UI::Image*> m_MainMenuButtons;
	UI::Text* m_HoverBoxText;
	std::string m_HoverText;
	bool m_IsHover;
	Vec2i m_MousePos;
	std::string buttonsInfo[5] = { "Play", "Select a level", "Settings", "Mute music", "Credits" };
	UI::Widget* m_Widget;
};