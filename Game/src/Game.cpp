#include <core/Objects/ObjectHandler.h>
#include <core/Utility/TimerManager.h>
#include <core/Common/defines.h>
#include <core/Drawable/Sprite.h>
#include <core/Common/Vec2.h>
#include <core/Drawable/Animation.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Events/EventManager.h>
#include <core/TiledMap/TiledMap.h>
#include "Game.h"
#include <iostream>
#include <UI/UIText.h>
#include <misc/Console/Console.h>
#include <UI/UIEditBox.h>
#include <core/Camera/Camera.h>
#include <core/Physics/Joints.h>
#include <core/TiledMap/Tilesets.h>

//UI::EditBox* editBox;
Camera* cam;
bool moveleft = true; 
bool anti_spam = false;

void Game::On_Engine_Pre_Init()  
{ 
	LogTerminal("Trikyta Engine Pre-initializing. ");
};

void Game::ManagerMechant()
{	float speedX= moveleft ? 25.f : -25.f;
	if (moveleft){
			speedX=-25.f;
			moveleft=false;
		}
		else{
			speedX=25.f;
			moveleft=true;
		}
	for (auto& b : mechantBody){
		moveleft ?
		dynamic_cast<Drawable*>(b->getChildrens()->at(0))->Flip(FLIPTYPE::NONE):
		dynamic_cast<Drawable*>(b->getChildrens()->at(0))->Flip(FLIPTYPE::HORIZONTAL);
		b->SetLinearVelocity(Vec2f(0.f,0.f));
		b->SetLinearVelocity(Vec2f(speedX,0.f));
	}
}
void Game::On_Engine_Init()
{
	cam = Camera::CreateCamera();
	map = TiledMap::Create("assets/example/maps/map/map.tmx");
	cam->addObjectToCamera(map);
	anim = Animation::Create("assets/anim_pack.png", "assets/anim_pack.a", Vec2i(256/5, 217/5), Vec2i(ENGINE->GetScreenWidth() / 2, (ENGINE->GetScreenHeight() / 2)-500), 0.03f);
	body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
	cam->addObjectToCamera(anim);
	anim->ToggleRotationAttachement(false);
	body->SetAngularDamping(1000.f);



	
	for (int i = 0; i < map->getMap()->GetNumObjectGroups(); ++i)
	{
		// Get an object group.
		const Tmx::ObjectGroup *objectGroup = map->getMap()->GetObjectGroup(i);
		// Iterate through all objects in the object group.
		if (objectGroup->GetName()=="tortue"){
			for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
				// Get an object.
				const Tmx::Object* object = objectGroup->GetObject(j);
				Vec2i pos = Vec2i(object->GetX(),object->GetY());
				auto animtortue = Animation::Create("assets/mechants/mechant.png", "assets/mechants/tortue.txt", Vec2i(40, 40), pos, 0.03f);
				cam->addObjectToCamera(animtortue);
				bodytortue = animtortue->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
				mechantBody.emplace_back(bodytortue);
				bodytortue->SetAngularDamping(1000.f);
				animtortue->ToggleRotationAttachement(false);
			}


		}
		if (objectGroup->GetName()=="squelette"){
			for (int m = 0; m < objectGroup->GetNumObjects(); ++m) {
				// Get an object.
				const Tmx::Object* object2 = objectGroup->GetObject(m);
				Vec2i pos2 = Vec2i(object2->GetX(),object2->GetY());
				auto animsquelette = Animation::Create("assets/chars/fzombie_female.png", "assets/chars/fzombie_walk.txt", Vec2i(40, 40), pos2, 0.03f);
				cam->addObjectToCamera(animsquelette);
				bodysquelette = animsquelette->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
				mechantBody.emplace_back(bodysquelette);
				bodysquelette->SetAngularDamping(1000.f);
				animsquelette->ToggleRotationAttachement(false);
			}


		} 
		if (objectGroup->GetName()=="bonus"){
			for (int k = 0; k<objectGroup->GetNumObjects(); k++){
				const Tmx::Object* object1 = objectGroup->GetObject(k);
				Vec2i pos1 = Vec2i(object1->GetX(),object1->GetY());
				auto animbonus = Animation::Create("assets/bonus/bonus.png", "assets/bonus/bonus.txt", Vec2i(20, 20), pos1, 0.03f);
				cam->addObjectToCamera(animbonus);
				bodybonus= animbonus->Physicalize(Physics2D::BodyParams{ 0, 0 }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0, 0));
			}
		}
	}
	auto t = TimerManager::CreateTimer(CALLBACK_0(Game::ManagerMechant,this), 1000, 0, true);
	//EVENT TESTING!!
	EventManager::GetEventManager()->addEventHandler<Events::ON_KEYBOARD_INPUT>(CALLBACK_2(Game::On_Input, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_START>(CALLBACK_1(Game::OnCollision, this));
	/*EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_END>(CALLBACK_1(Game::OnCollisionEnd, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_CLICK>(CALLBACK_3(Game::OnClick, this));*/
	//EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_MOVE>(CALLBACK_1(Game::OnMouseMove, this));
};


void Game::On_Engine_Render(float /*dt*/)
{
	// Offset the player quad by the camera position 
	bool b = true;
	Vec2i pos = Vec2i((int)body->GetPosition().x, (int)body->GetPosition().y) - Vec2i(cam->getCameraSize().x/2, cam->getCameraSize().y/2);
	if (b)
		cam->setCameraPosition(pos);
};

void Game::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{ 
	if (p_KeyState == SDL_KEYDOWN) {
		if (p_Key == SDLK_RIGHT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::NONE);
		}else if (p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = -50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::HORIZONTAL);
		}else if (p_Key == SDLK_UP) {
			//body->ApplyLinearImpulse(Vec2f(0, body->GetMass() * -1000.f), body->GetWorldCenter(), false);
			Vec2f v = body->GetLinearVelocity();
			v.y = -50.f;
			body->SetLinearVelocity(v);
		}else if (p_Key == SDLK_DOWN) {
			
		}else if (p_Key == SDLK_c) {

		}
	}
	
};

void Game::On_Engine_Quit() 
{ 
	EngineInstance::On_Engine_Quit();
	Log("Leaving game! "); 
};

void Game::OnCollision(b2Contact* contact)
{
	auto bodyA = contact->GetFixtureA()->GetPhysicsBody();
	auto bodyB = contact->GetFixtureB()->GetPhysicsBody();
	bool isACoin = map->isBodyPartOfTileset(bodyA, "misc2", 3) || map->isBodyPartOfTileset(bodyB, "misc2", 3);
	bool isARedSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 2) || map->isBodyPartOfTileset(bodyB, "misc2", 2);
	bool isAGreenSwitch = map->isBodyPartOfTileset(bodyA, "misc2", 1) || map->isBodyPartOfTileset(bodyB, "misc2", 1);
	bool isAHeart = map -> isBodyPartOfTileset (bodyA ,"misc2", 5) || map -> isBodyPartOfTileset(bodyB , "misc2" , 5);
	bool isAKey = map -> isBodyPartOfTileset (bodyA ,"sheet_key", 0) || map -> isBodyPartOfTileset(bodyB , "sheet_key" , 0);
	if (isACoin) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a coin !");
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->deleteTileInLayer(tileToDelete);
				LogConsole(LogWarning, "Contact with a coin !");
			}
		}
	}else if (isARedSwitch && !anti_spam) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 1, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a red switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}
	}else if (isAGreenSwitch && !anti_spam) {
		if (bodyB == body) {
			LayerData* tileToDelete = bodyA->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 2, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}else if (bodyA == body) {
			LayerData* tileToDelete = bodyB->getComponent<LayerData>();
			if (tileToDelete != NULL) {
				map->addTileToLayer(map->getTilset("misc2"), 2, "switch", tileToDelete->tiledLayerData->m_MapGrid);
				LogConsole(LogWarning, "Contact with a green switch !");
				anti_spam = true;
				TimerManager::CreateTimer([]() {anti_spam = false; }, 3000, 1, true);
			}
		}
	}else if(isAHeart){
	    if (bodyA == body){
		LayerData* tileToDelete = bodyA -> getComponent<LayerData>();
		if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a heart !");
			//mettre compteur 
		}
		}else if (bodyB == body) {
		  LayerData* tileToDelete = bodyA->getComponent<LayerData>();
		      if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a heart !");
		      }
		}
	}else if(isAKey){
	  if (bodyB == body){
		LayerData* tileToDelete = bodyA -> getComponent<LayerData>();
		if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a key !");
		}
		}else if (bodyA == body) {
		  LayerData* tileToDelete = bodyA->getComponent<LayerData>();
		      if (tileToDelete != NULL) {
			map->deleteTileInLayer(tileToDelete);
			LogConsole(LogWarning, "Contact with a key !");
		      }
		}
	}
	if (body==bodytortue){
		LogConsole(LogWarning, "Contact with a turtel !");
		printf("hello");
	}
	if (body==bodysquelette){
		LogConsole(LogWarning, "Contact with a zombie !");
	}
	if (body==bodybonus){
		LogConsole(LogWarning, "Contact with a bonus!");
	}


}

void Game::OnCollisionEnd(b2Contact* contact)
{
	Log("TWO ENDED BODIED COLLISION!!")
}

void Game::OnClick(unsigned int EventType, int button, Vec2i pos)
{
	if (EventType == SDL_MOUSEBUTTONDOWN && button == SDL_BUTTON_RIGHT) {
		Log("CLICK : (%d, %d)", pos.x, pos.y)
	}
}

void Game::OnMouseMove(Vec2i pos)
{
	Log("Mouse moved (%d, %d)", pos.x, pos.y);
}
