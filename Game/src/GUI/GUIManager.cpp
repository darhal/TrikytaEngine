#include "GUIManager.hpp"
#include <sound/Music.hpp>
#include "MainMenu.hpp"
#include "LoadingMenu.hpp"
#include <core/Utility/TimerManager.h>
#include <sound/SoundEffect.hpp>
#include "InGameMenu.hpp"
#include "../GameManager/GameManager.hpp"

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
	m_MainMusic->Play(-1);
	m_MainMusic->setVolume(50);
	m_ClickEffect = new SoundEffect("assets/gui/click_effect.wav");
	GUIManager::GoTo(MAIN_MENU);
}

void GUIManager::ReInit()
{
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/sky.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/ground_3.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/plant.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/clouds_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC);
	bgManager.Add(Sprite::Create("assets/gui/background/game_background_3/layers/clouds_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC);
	m_MainMusic->Play(-1);
	m_MainMusic->setVolume(50);
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
		TimerManager::CreateTimer([=]() {FREE(m_CurrentMenu); bgManager.Clear(); } , 200, 1, true);
		TimerManager::CreateTimer([=]() { GoTo(Goto); }, 500, 1, true);
	}else if (left == INGAME_MENU) {
		TimerManager::CreateTimer([=]() { 
			if (dynamic_cast<InGameMenu*>(m_CurrentMenu) != nullptr)
			{
				auto ptr = dynamic_cast<InGameMenu*>(m_CurrentMenu)->m_GameManager;
				FREE(ptr);
			}
		}, 50, 1, true);
		TimerManager::CreateTimer([=]() { bgManager.Clear();  }, 150, 1, true);
		TimerManager::CreateTimer([=]() { ReInit(); GoTo(Goto); }, 250, 1, true);
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