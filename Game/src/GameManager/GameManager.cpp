#include "GameManager.hpp"
#include <core/TiledMap/TiledMap.h>
#include "../GUI/LoadingMenu.hpp"
#include "../GUI/GUIManager.hpp"
#include <core/Camera/Camera.h>
#include <core/Physics/Joints.h>
#include <core/Common/defines.h>
#include <sound/SoundEffect.hpp>
#include <core/Drawable/Sprite.h>
#include <misc/Console/Console.h>
#include <core/TiledMap/TiledMap.h>
#include <core/TiledMap/Tilesets.h>
#include <core/Drawable/Animation.h>
#include <core/Events/EventManager.h>
#include <core/Utility/TimerManager.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Objects/ObjectHandler.h>
#include <core/Drawable/AnimationSet.hpp>
#include "PlayGame.hpp"
#include <UI/UIBase.h>
#include <UI/UIManager.h>

GameManager::GameManager(GUIManager* GUI_Manager, LoadingMenu* lm) : m_GUIManager(GUI_Manager), m_LoadingMenu(lm)
{
	m_Game = new PlayGame(this);
}

GameManager::~GameManager()
{
	FREE(m_Game);
}

void GameManager::InitGame()
{
	if (m_LoadingMenu == nullptr) { return; }
	m_Game->Init(m_LoadingMenu);
}

void GameManager::BeginPlay()
{
	TimerManager::CreateTimer([&]() {
		m_GUIManager->bgManager.QueueClear();
		m_GUIManager->MuteMusic(true);
		m_GUIManager->GoTo(NO_MENU);
		FREE(m_LoadingMenu); m_GUIManager->m_CurrentMenu = nullptr; 
		for (auto itr : m_Game->m_ObjectsCreated) {
			ObjectHandler::SetObjectSleeping(itr, true);
		};
		ENGINE->AllowPhysicsStepping(true);
		AddEventHandler(ON_COLLISION_START, CALLBACK_1(PlayGame::Collision, m_Game));
		AddEventHandler(ON_ENGINE_RENDER, CALLBACK_1(PlayGame::Render, m_Game));
		AddEventHandler(ON_KEYBOARD_INPUT, CALLBACK_2(PlayGame::On_Input, m_Game));
		LogConsole(LogInfo, "Game begin good luck!");
	}, 1000, 1);
}

