#pragma once
#include "Background.hpp"

enum UI_MENUS
{
	MAIN_MENU,
	LOADING_MENU,
	INGAME_MENU,
	LEVEL_MENU,
	NONE,
};

class GUIManager
{
public:
	GUIManager();
	void Refresh(float dt);
	void MuteMusic(bool is_mute);
	class Music* getMainMusic() { return m_MainMusic; }
	void OnLeaveMenu(UI_MENUS, UI_MENUS);
	void GoTo(UI_MENUS);
	class BaseMenu* GetCurrentMenu();
	class Background bgManager;
	class GameManager* m_GameManager;
	UI_MENUS m_MenuType;
private:
	class Music* m_MainMusic;
	class BaseMenu* m_CurrentMenu;
};