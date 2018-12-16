#include "LoadingMenu.hpp"
#include "GUIManager.hpp"
#include "Background.hpp"
#include <UI/UIProgressbar.hpp>
#include <misc/Font.h>
#include <core/Utility/TimerManager.h>
#include "../GameManager/GameManager.hpp"
#include <thread>
#include <future>

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
	//std::thread t;
	if (m_ProgressBar != nullptr) {
		ENGINE->AllowPhysicsStepping(false);
		GUI_Manager->m_GameManager = new GameManager(GUI_Manager, this);	
		GUI_Manager->m_GameManager->InitGame();
		//t = std::thread(GUI_Manager->m_GameManager->InitGame);
		//t.detach();
	}else{
		LogTerminal("Progress bar is nullptr");
	}
}

void LoadingMenu::AddProgress(int p)
{
	if (m_ProgressBar == nullptr) { return; }
	m_ProgressBar->setProgress(m_ProgressBar->getProgress() + p, Color{ 249, 67, 103, 255 }, Color{ 249, 0, 49, 255 }, 1);
	if (m_ProgressBar != nullptr && m_ProgressBar->getProgress() >= 100) {
		GUI_Manager->m_GameManager->BeginPlay();
		return;
	}
}

LoadingMenu::~LoadingMenu()
{
	m_ProgressBar->setVisible(false);
	FREE(m_ProgressBar);
}