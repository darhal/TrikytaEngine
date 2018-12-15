#include "LoadingMenu.hpp"
#include "GUIManager.hpp"
#include "Background.hpp"
#include <UI/UIProgressbar.hpp>
#include <misc/Font.h>
#include <core/Utility/TimerManager.h>
#include "../GameManager/GameManager.hpp"

LoadingMenu::LoadingMenu(class GUIManager* manager) : BaseMenu(manager)
{
	manager->bgManager

	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/clouds_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/clouds_3.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/clouds_4.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), DYNAMIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/clouds_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/rocks_2.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/rocks_1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC)
	.Add(Sprite::Create("assets/gui/background/game_background_1/layers/sky.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0)), STATIC);
	LoadingMenu::BuildLoadingMenu();
}

void LoadingMenu::BuildLoadingMenu()
{
	using namespace UI;
	m_ProgressBar = new Progressbar(Vec2i(10, 150), Vec2i(ENGINE->GetScreenWidth() - ENGINE->GetScreenWidth()/8, 45), Color(0, 0, 0, 200), Color(98, 204, 239, 255), "Loading...", Color{ 0,0,0,255 }, Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 28));
	m_ProgressBar->setPos(Vec2i((ENGINE->GetScreenWidth() - m_ProgressBar->getSize().x) / 2, int(ENGINE->GetScreenHeight() - m_ProgressBar->getSize().y*1.3f)));
	GUI_Manager->m_GameManager = new GameManager(GUI_Manager);
	if (m_ProgressBar != nullptr) {
		
	}else{
		LogTerminal("Progress bar is nullptr");
	}
}

void LoadingMenu::AddProgress(int p)
{
	m_ProgressBar->setProgress(m_ProgressBar->getProgress() + p, Color{ 249, 67, 103, 255 }, Color{ 249, 0, 49, 255 }, 1);
}