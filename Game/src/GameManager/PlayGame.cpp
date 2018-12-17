#include "PlayGame.hpp"
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
#include "../GUI/LoadingMenu.hpp"
#include <UI/UIText.h>
#include "GameManager.hpp"
#include "../GUI/GUIManager.hpp"

PlayGame::PlayGame(GameManager* m) : m_GameManager(m)
{
	m_InGameMainMenu = new InGameMenu(m_GameManager->m_GUIManager, m);
	moveleft = true;
	anti_spam = false;
	compteurCoeur = 3;
	compteurPiece = 0;
	compteurKey = 0;
	cam = nullptr;
	body = nullptr;
	m_ReadToStartGame = false;
}

PlayGame::~PlayGame()
{
	ClearAllEventHandlers(ON_COLLISION_START);
	ClearAllEventHandlers(ON_ENGINE_RENDER);
	ClearAllEventHandlers(ON_KEYBOARD_INPUT);
	m_Timer->stop();
	FREE(m_InGameMainMenu);
	for (auto itr : m_ObjectsCreated) {
		FREE(itr);
	}
	FREE(cam);
}

void PlayGame::ManagerMechant()
{
	float speedX = moveleft ? 25.f : -25.f;
	if (moveleft) {
		speedX = -25.f;
		moveleft = false;
	}
	else {
		speedX = 25.f;
		moveleft = true;
	}
	for (auto& b : mechantBody) {
		moveleft ?
			dynamic_cast<Drawable*>(b->getChildrens()->at(0))->Flip(FLIPTYPE::NONE) :
			dynamic_cast<Drawable*>(b->getChildrens()->at(0))->Flip(FLIPTYPE::HORIZONTAL);
		b->SetLinearVelocity(Vec2f(0.f, 0.f));
		b->SetLinearVelocity(Vec2f(speedX, 0.f));
	}
}

void PlayGame::Init(LoadingMenu* m_LoadingMenu)
{
	m_Background = Sprite::Create("assets/background.png", Vec2i(ENGINE->GetScreenWidth(), (ENGINE->GetScreenHeight())), Vec2i(0,0), false);
	m_ObjectsCreated.emplace_back(m_Background);
	cam = Camera::CreateCamera();
	cam->setCameraPosition(Vec2i(0, 0));
	m_LoadingMenu->AddProgress(5);

	TimerManager::CreateTimer([=]() { 
		map = TiledMap::Create("assets/map/map.tmx", false);
		cam->addObjectToCamera(map);
		m_LoadingMenu->AddProgress(40);
		m_ObjectsCreated.emplace_back(map);
	}, 150, 1);

	TimerManager::CreateTimer([=]() {
		anim = AnimationSet::Create("assets/player.png", "assets/player.txt", Vec2i(256 / 7, 217 / 7), Vec2i(ENGINE->GetScreenWidth() / 2, (ENGINE->GetScreenHeight() / 2) - 500), 0.03f, false);
		cam->addObjectToCamera(anim);
		m_LoadingMenu->AddProgress(10);
		m_ObjectsCreated.emplace_back(anim);
	}, 2150, 1);

	TimerManager::CreateTimer([=]() {
		body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
		m_LoadingMenu->AddProgress(14);
		anim->ToggleRotationAttachement(false);
		body->SetAngularDamping(1000.f);
		anim->setAnimation("Idle");
		m_LoadingMenu->AddProgress(5);
	}, 3150, 1);


	TimerManager::CreateTimer([=]() {
		int po = 0;
		for (int i = 0; i < 4; i++) {
			auto Coeur = Sprite::Create("assets/bonus/02_heart.png", Vec2i(25, 22), Vec2i(ENGINE->GetScreenWidth() - po, 0), false);
			po = po + 22;
			coeurs[i] = Coeur;
			m_ObjectsCreated.emplace_back(Coeur);
		}
		m_LoadingMenu->AddProgress(5);
	}, 4150, 1);

	TimerManager::CreateTimer([=]() {
		auto Piece = Sprite::Create("assets/PNG/Gold/Gold_1.png", Vec2i(25, 22), Vec2i(20, 0), false);
		char buffer[256];
		sprintf(buffer, "Score: %d", compteurPiece);
		pieceScore = UI::Text::createText(buffer, "Engine_Assets/fonts/DroidSans.ttf", 18, Vec2i(45, 0), { 255,255,255,255 }, 0, false);
		auto Key = Sprite::Create("assets/key.png", Vec2i(25, 22), Vec2i(ENGINE->GetScreenWidth() / 2, 0), false);
		sprintf(buffer, "%d /3", compteurKey);
		keyScore = UI::Text::createText(buffer, "Engine_Assets/fonts/DroidSans.ttf", 18, Vec2i((ENGINE->GetScreenWidth() / 2) + 25, 0), { 255,255,255,255 }, 0, false);
		m_LoadingMenu->AddProgress(10);
		m_ObjectsCreated.emplace_back(Key);
		m_ObjectsCreated.emplace_back(Piece);
	}, 5150, 1);

	TimerManager::CreateTimer([=]() {
	for (int i = 0; i < map->getMap()->GetNumObjectGroups(); ++i)
	{
		// Get an object group.
		const Tmx::ObjectGroup *objectGroup = map->getMap()->GetObjectGroup(i);
		// Iterate through all objects in the object group.
		if (objectGroup->GetName() == "tortue") {
			for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
				// Get an object.
				const Tmx::Object* object = objectGroup->GetObject(j);
				Vec2i pos = Vec2i(object->GetX(), object->GetY());
				auto animtortue = Animation::Create("assets/mechants/mechant.png", "assets/mechants/tortue.txt", Vec2i(40, 40), pos, 0.03f, false);
				cam->addObjectToCamera(animtortue);
				auto bodytortue = animtortue->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
				mechantBody.emplace_back(bodytortue);
				bodytortue->SetAngularDamping(1000.f);
				animtortue->ToggleRotationAttachement(false);
				m_ObjectsCreated.emplace_back(animtortue);
			}
		}
		if (objectGroup->GetName() == "squelette") {
			for (int m = 0; m < objectGroup->GetNumObjects(); ++m) {
				// Get an object.
				const Tmx::Object* object2 = objectGroup->GetObject(m);
				Vec2i pos2 = Vec2i(object2->GetX(), object2->GetY());
				auto animsquelette = Animation::Create("assets/chars/fzombie_female.png", "assets/chars/fzombie_walk.txt", Vec2i(40, 40), pos2, 0.03f, false);
				cam->addObjectToCamera(animsquelette);
				auto bodysquelette = animsquelette->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
				mechantBody.emplace_back(bodysquelette);
				bodysquelette->SetAngularDamping(1000.f);
				animsquelette->ToggleRotationAttachement(false);
				m_ObjectsCreated.emplace_back(animsquelette);
			}
		}
	}
	m_ReadToStartGame = true;
	TimerManager::CreateTimer([m_LoadingMenu]() {
		if (m_LoadingMenu != nullptr) {
			m_LoadingMenu->AddProgress(25); 
		} 
		}, 600, 1);
	}, 5150, 1);
	m_Timer = TimerManager::CreateTimer(CALLBACK_0(PlayGame::ManagerMechant, this), 1000, 0, true);
}

void PlayGame::Collision(b2Contact* contact)
{
	auto bodyA = contact->GetFixtureA()->GetPhysicsBody();
	auto bodyB = contact->GetFixtureB()->GetPhysicsBody();
	bool isACoin = map->isBodyPartOfTileset(bodyA, "misc2", 3) || map->isBodyPartOfTileset(bodyB, "misc2", 3);
	bool isARedSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 2) || map->isBodyPartOfTileset(bodyB, "misc2", 2);
	bool isAGreenSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 1) || map->isBodyPartOfTileset(bodyB, "misc2", 1);
	bool isAHeart = map->isBodyPartOfTileset(bodyA, "misc2", 5) || map->isBodyPartOfTileset(bodyB, "misc2", 5);
	bool isAKey = map->isBodyPartOfTileset(bodyA, "sheet_key", 0) || map->isBodyPartOfTileset(bodyB, "sheet_key", 0);
	bool isADoor = map->isBodyPartOfTileset(bodyA, "misc", 62) || map->isBodyPartOfTileset(bodyA, "misc", 61) || map->isBodyPartOfTileset(bodyA, "misc", 60) || map->isBodyPartOfTileset(bodyA, "misc", 51) || map->isBodyPartOfTileset(bodyA, "misc", 50) || map->isBodyPartOfTileset(bodyA, "misc", 49);
	bool isBDoor = map->isBodyPartOfTileset(bodyB, "misc", 62) || map->isBodyPartOfTileset(bodyB, "misc", 61) || map->isBodyPartOfTileset(bodyB, "misc", 60) || map->isBodyPartOfTileset(bodyB, "misc", 51) || map->isBodyPartOfTileset(bodyB, "misc", 50) || map->isBodyPartOfTileset(bodyB, "misc", 49);
	bool isEpine = map->isBodyPartOfTileset(bodyA, "misc", 47) || map->isBodyPartOfTileset(bodyB, "misc", 47);
	if (isACoin) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				compteurPiece++;
				char buffer[256];
				sprintf(buffer, "Score: %d", compteurPiece);
				pieceScore->updateText(buffer);
			}

		}
		else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				compteurPiece++;
				char buffer[256];
				sprintf(buffer, "Score: %d", compteurPiece);
				pieceScore->updateText(buffer);
			}
		}
	}
	else if (isARedSwitch && !anti_spam) {
		/*if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([this]() {anti_spam = false; }, 3000, 1, true);
			}
		}
		else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([this]() {anti_spam = false; }, 3000, 1, true);
			}
		}*/
	}
	else if (isAGreenSwitch && !anti_spam) {
		/*if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 2, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([this]() {anti_spam = false; }, 3000, 1, true);
			}
		}
		else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 2, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([this]() {anti_spam = false; }, 3000, 1, true);
			}
		}*/
	}
	else if (isAHeart) {
		if (bodyA == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a heart !");
				if (compteurCoeur < 3) {
					LogConsole(LogWarning, "Incr�mentation");
					compteurCoeur++;
					coeurs[compteurCoeur]->setRender(true);
					//ObjectHandler::SetObjectSleeping(coeurs[compteurCoeur], true);
				}
			}
		}
		else if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a heart !");
				if (compteurCoeur < 3) {
					compteurCoeur++;
					coeurs[compteurCoeur]->setRender(true);
					//ObjectHandler::SetObjectSleeping(coeurs[compteurCoeur], true);
				}
			}
		}
	}
	else if (isAKey) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				compteurKey++;
				char buffer[256];
				sprintf(buffer, "%d /3", compteurKey);
				keyScore->updateText(buffer);
			}
		}
		else if (bodyA == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				compteurKey++;
				char buffer[256];
				sprintf(buffer, "%d /3 ", compteurKey);
				keyScore->updateText(buffer);
			}
		}
	}else if (isADoor || isBDoor) {
		if (compteurKey == 3) {
			if (bodyB == body) {
				ClearAllEventHandlers(ON_COLLISION_START);
				ClearAllEventHandlers(ON_ENGINE_RENDER);
				ClearAllEventHandlers(ON_KEYBOARD_INPUT);
				LogTerminal("You win!");
				m_GameManager->m_GUIManager->OnLeaveMenu(INGAME_MENU, MAIN_MENU);
			}else if (body == bodyA) {
				ClearAllEventHandlers(ON_COLLISION_START);
				ClearAllEventHandlers(ON_ENGINE_RENDER);
				ClearAllEventHandlers(ON_KEYBOARD_INPUT);
				LogTerminal("You win!");
				m_GameManager->m_GUIManager->OnLeaveMenu(INGAME_MENU, MAIN_MENU);
			}
		}
	}
	else if (isEpine) {
		if (compteurCoeur > 0) {
			coeurs[compteurCoeur]->setRender(false);
			//ObjectHandler::SetObjectSleeping(coeurs[compteurCoeur], false);
			compteurCoeur--;
		}
	}
	if ((body == bodyB) && (std::find(mechantBody.begin(), mechantBody.end(), bodyA) != mechantBody.end())) {
		if (compteurCoeur > 0) {
			anim->Flip(FLIPTYPE::NONE);
			anim->setAnimation("Dead");
			coeurs[compteurCoeur]->setRender(false);
			//ObjectHandler::SetObjectSleeping(coeurs[compteurCoeur], false);
			compteurCoeur--;
		}
	}
	else if ((body == bodyA) && (std::find(mechantBody.begin(), mechantBody.end(), bodyB) != mechantBody.end())) {
		if (compteurCoeur > 0) {
			anim->Flip(FLIPTYPE::NONE);
			anim->setAnimation("Dead");
			coeurs[compteurCoeur]->setRender(false);
			//ObjectHandler::SetObjectSleeping(coeurs[compteurCoeur], false);
			compteurCoeur--;
		}
	}
	if (compteurCoeur == 0) {
		LogConsole(LogWarning, "game over");
		ClearAllEventHandlers(ON_COLLISION_START);
		ClearAllEventHandlers(ON_ENGINE_RENDER);
		ClearAllEventHandlers(ON_KEYBOARD_INPUT);
		m_GameManager->m_GUIManager->OnLeaveMenu(INGAME_MENU, MAIN_MENU);
	}
}

void PlayGame::Render(float)
{
	// Offset the player quad by the camera position 
	Vec2i pos = Vec2i((int)body->GetPosition().x, (int)body->GetPosition().y) - Vec2i(cam->getCameraSize().x / 2, cam->getCameraSize().y / 2);
	cam->setCameraPosition(pos);
	keyScore->render(0.f);
	pieceScore->render(0.f);
}

void PlayGame::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{
	if (p_KeyState == SDL_KEYDOWN) {
		if (p_Key == SDLK_RIGHT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::NONE);
			anim->setAnimation("Run");
		}
		else if (p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = -50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::HORIZONTAL);
			anim->setAnimation("Run");
		}
		else if (p_Key == SDLK_UP) {
			//body->ApplyLinearImpulse(Vec2f(0, body->GetMass() * -1000.f), body->GetWorldCenter(), false);
			Vec2f v = body->GetLinearVelocity();
			v.y = -50.f;
			body->SetLinearVelocity(v);
			anim->setAnimation("Jump");
		}
		else if (p_Key == SDLK_DOWN) {

		}
		else if (p_Key == SDLK_c) {

		}
		else if (p_Key == SDLK_ESCAPE && m_GameManager != nullptr) {
			if (m_GameManager->m_GUIManager->m_MenuType == INGAME_MENU) {
				m_InGameMainMenu->ShowMenu(false);
				LogTerminal("Showing main menu!");
			}else if (m_GameManager->m_GUIManager->m_MenuType == NO_MENU){
				m_InGameMainMenu->ShowMenu(true);
				LogTerminal("Hiding main menu!");
			}
		}
	}
	else if (p_KeyState == SDL_KEYUP) {
		if (p_Key == SDLK_RIGHT || p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 0.f;
			body->SetLinearVelocity(v);
			anim->setAnimation("Idle");
		}
		else if (p_Key == SDLK_UP) {
			anim->setAnimation("Fall");
		}
	}
	;;;;;;;;;;;;;;
};