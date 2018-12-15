#include "GUIManager.hpp"
#include <sound/Music.hpp>
#include "MainMenu.hpp"
#include "LoadingMenu.hpp"
#include <core/Utility/TimerManager.h>

GUIManager::GUIManager()
{
	bgManager.SetEffectValue(Vec2i(1, 0));
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/sky.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_3.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/plant.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/clouds_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/clouds_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC);

	m_MainMusic = new Music("assets/gui/main_theme.mp3");
	m_MainMusic->setVolume(75);
	m_MainMusic->Play(-1);

	GUIManager::GoTo(MAIN_MENU);
}

void GUIManager::Refresh(float dt)
{
	bgManager.Refresh(dt);
}

void GUIManager::MuteMusic(bool is_mute)
{
	if (is_mute)
		m_MainMusic->Stop();
	else
		m_MainMusic->Play(-1);
}

void GUIManager::OnLeaveMenu(UI_MENUS left, UI_MENUS Goto)
{
	if (left == MAIN_MENU) {
		TimerManager::CreateTimer([=]() {FREE(m_CurrentMenu); bgManager.Clear(); } , 50, 1, true);
		TimerManager::CreateTimer([=]() { GoTo(Goto); }, 100, 1, true);
	}
}

void GUIManager::GoTo(UI_MENUS menu)
{
	m_MenuType = menu;
	if (menu == MAIN_MENU) {
		m_CurrentMenu = new MainMenu(this);
	}else if (menu == LOADING_MENU) {
		m_CurrentMenu = new LoadingMenu(this);
	}
}

BaseMenu* GUIManager::GetCurrentMenu()
{
	return m_CurrentMenu;
}