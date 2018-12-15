#include "GameManager.hpp"
#include "../GUI/LoadingMenu.hpp"
#include <core/TiledMap/TiledMap.h>
#include "../GUI/GUIManager.hpp"
#include <core/Drawable/AnimationSet.hpp>

GameManager::GameManager(GUIManager* GUI_Manager) : m_GUIManager(GUI_Manager)
{
	
}

void GameManager::InitGame()
{
	LoadingMenu* loadingMenu = dynamic_cast<LoadingMenu*>(m_GUIManager->GetCurrentMenu());
	TiledMap* map = TiledMap::Create("assets/map/map.tmx");
	loadingMenu->AddProgress(15);
}