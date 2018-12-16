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

GameManager::GameManager(GUIManager* GUI_Manager, LoadingMenu* lm) : m_GUIManager(GUI_Manager), m_LoadingMenu(lm)
{
	TimerManager::CreateTimer([&]() {GameManager::InitGame(); }, 50, 1);
}

void GameManager::InitGame()
{
	if (m_LoadingMenu == nullptr) { return; }
	cam = Camera::CreateCamera();
	m_LoadingMenu->AddProgress(10);
	TimerManager::CreateTimer([=]() { 
		map = TiledMap::Create("assets/map/map.tmx", false); 
		map->setRender(false);
		cam->addObjectToCamera(map);
		m_LoadingMenu->AddProgress(75);
	}, 150, 1);
	TimerManager::CreateTimer([=]() {
		anim = AnimationSet::Create("assets/player.png", "assets/player.txt", Vec2i(256 / 7, 217 / 7), Vec2i(ENGINE->GetScreenWidth() / 2, (ENGINE->GetScreenHeight() / 2) - 500), 0.03f, false);
		anim->setRender(false);
		cam->addObjectToCamera(anim);
		m_LoadingMenu->AddProgress(20);
	}, 2400, 1);
	TimerManager::CreateTimer([=]() {
		auto body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
		m_LoadingMenu->AddProgress(14);
		anim->ToggleRotationAttachement(false);
		body->SetAngularDamping(1000.f);
		anim->setAnimation("Idle");
		m_LoadingMenu->AddProgress(1);
	}, 2800, 1);
	
}

void GameManager::BeginPlay()
{
	TimerManager::CreateTimer([&]() {
		ENGINE->AllowPhysicsStepping(true);
		m_GUIManager->bgManager.QueueClear();
		m_GUIManager->MuteMusic(true);
		m_GUIManager->GoTo(NO_MENU);
		FREE(m_LoadingMenu); m_GUIManager->m_CurrentMenu = nullptr; 
		map->setRender(true);
		anim->setRender(true);
	}, 1000, 1);
}

