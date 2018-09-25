#include <core/Common/defines.h>
#include <core/Drawable/Sprite.h>
#include <core/Common/Vec2.h>
#include <core/Drawable/Animation.h>
#include <core/Physics/PhysicsEngine.h>
#include <core/Events/EventManager.h>
#include <core/TiledMap/TiledMap.h>
#include <core/Objects/ObjectHandler.h>
#include "Game.h"
#include <iostream>
#include <core/Utility/TimerManager.h>

void Game::On_Engine_Pre_Init()  
{ 
	Log("Trikyta Engine Pre-initializing. "); 
};

void Game::On_Engine_Init()
{

	//for (int i = 0; i < 100; i++) {
	//auto t = TimerManager::CreateTimer(f, 1000, 3, true);
	//}
	//TILED MAP TEST
	/*map = TiledMap::Create("assets/example/maps/map.tmx");

	//sprite test
	obj = Sprite::Create("assets/test.png", Vec2i(464, 464), Vec2i(50, 100));
	Log("Sprite obj : %s", obj->getFileName().c_str());

	//Animation test
	anim = Animation::Create("assets/anim_pack.png", "assets/anim_pack.a", Vec2i(0, 0), Vec2i(650, 75), 0.03f);
	Log("Animation obj : %s", anim->getFileName().c_str());

	using namespace Physics2D;
	// Body creation test
	auto Ground = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::STATIC,
		BodyShape::BOX, BodyParams{ 1,1 },
		Vec2f{0.f, (float)(this->GetScreenHeight()+ 464 / PTM)-150.f },
		std::vector<Vec2f>{Vec2f((float)this->GetScreenWeight(), 464/ PTM)}
	);
	
	body = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::DYNAMIC,
		BodyShape::BOX, BodyParams{3.0f,0.1f},
		Vec2f{ 50 + obj->getSize().x/PTM, 100+obj->getSize().y/PTM },
		std::vector<Vec2f>{Vec2f(obj->getSize().x/PTM, obj->getSize().y/PTM)}
	);

	body2 = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::DYNAMIC,
		BodyShape::BOX, BodyParams{ 1.f,0.1f },
		Vec2f{ 650 + anim->getDrawSize().x/PTM, 75 + anim->getDrawSize().y/PTM },
		std::vector<Vec2f>{Vec2f((anim->getDrawSize().x-100)/PTM, (anim->getDrawSize().y-55)/PTM)}
	);

	//Phy simulation test
	body2->SetLinearVelocity(b2Vec2(-25.f, 0.f));
	//body->SetLinearVelocity(b2Vec2(25.f, 0.f));
	body2->SetAngularDamping(6.f);

	// attaching testing
	//obj->attachTo(body, Vec2f(-0.5f,-0.5f));
	anim->attachTo(body2, Vec2f(-0.5f, -0.5f));
	//EVENT TESTING!!
	EventManager::GetEventManager()->addEventHandler<Events::ON_KEYBOARD_INPUT>(CALLBACK_2(Game::On_Input, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_START>(CALLBACK_1(Game::OnCollision, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_END>(CALLBACK_1(Game::OnCollisionEnd, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_CLICK>(CALLBACK_3(Game::OnClick, this));*/
	//EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_MOVE>(CALLBACK_1(Game::OnMouseMove, this));
};

void Game::On_Engine_Render(float dt)
{
	//LuaBinds::OnRender(dt);
	/*if (obj->getPosition().x > obj->getSize().x + this->GetScreenWeight()) {
		obj->setPositionX(-(obj->getSize().x));
	}
	if (obj->getPosition().y > this->GetScreenHeight()+ obj->getSize().y) {
		obj->setPositionY(-obj->getSize().y);
	}*/
		
	/*if (anim->getPosition().x > anim->getSize().x + this->GetScreenWeight()) {
		anim->setPositionX(-(anim->getSize().x));
	}
	if (anim->getPosition().y > this->GetScreenHeight() + anim->getSize().y) {
		anim->setPositionY(-anim->getSize().y);
	}*/
	//anim->setPosition(Vec2i(anim->getPosition().x+1, anim->getPosition().y+0));
	//obj->setPosition(obj->getPosition() + Vec2i(1, 0));
};

void Game::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{ 
	if (p_KeyState == SDL_KEYUP) { return; }
	if (p_Key == SDLK_RIGHT) {
		/*anim->setPosition(Vec2i(anim->getPosition().x + 10, anim->getPosition().y + 0));
		anim->Flip(FLIPTYPE::NONE);*/
	}
	else if (p_Key == SDLK_LEFT) {
		/*anim->setPosition(Vec2i(anim->getPosition().x - 10, anim->getPosition().y + 0));
		anim->Flip(FLIPTYPE::HORIZONTAL);*/
	}
	else if (p_Key == SDLK_c) {
		/*if (!f) {
			FREE(map);
			f = true;
		}*/
	}
};

void Game::On_Engine_Quit() 
{ 
	Log("Leaving game! "); 
};

void Game::OnCollision(b2Contact* contact)
{
	Log("TWO BODIED COLLISION!!")
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
