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
UI::EditBox* editBox;

void Game::On_Engine_Pre_Init()  
{ 
	LogTerminal("Trikyta Engine Pre-initializing. ");
};

void Game::On_Engine_Init()
{
	editBox = UI::EditBox::createEditBox("ENTER SMTHG", "Engine_Assets/fonts/DroidSans.ttf", 18,
		Vec2i(START_POS_X, Console::getConsole()->getStartYPos()), Color{ 255,255,255, 255 });
	editBox->getText()->setBackgroundColor(Color{ 0,0,0,255 });

	//for (int i = 0; i < 100; i++) {
	//auto t = TimerManager::CreateTimer(f, 1000, 3, true);
	//}
	//TILED MAP TEST
	//map = TiledMap::Create("assets/example/maps/map.tmx");//"assets/example/maps/map.tmx");
	
	//std::string lol = "FPS: " + std::to_string(1));
	//a->updateText(lol);

	//sprite test
	/*obj = Sprite::Create("assets/test.png", Vec2i(464, 464), Vec2i(0, 0));
	//Log("Sprite obj : %s", obj->getFileName().c_str());
	//obj->Physicalize({ 1.f, 1.f }, Physics2D::BodyType::DYNAMIC);

	text = UI::Text::createText("This is a simple output example", "Engine_Assets/fonts/DroidSans.ttf", 12, Vec2i(0, 50), Color{ 255,255,255, 255 });
	text->setScale(1);
	text->setColor(Color(0, 255, 0, 255));

	anim = Animation::Create("assets/anim_pack.png", "assets/anim_pack.a", Vec2i(0, 0), Vec2i(650, 75), 0.03f);
	Log("Animation obj : %s", anim->getFileName().c_str());*/
	/*text->attachTo(anim, Vec2f(0.2f,0.f));
	anim->Physicalize(1.f, 1.f, Physics2D::BodyType::DYNAMIC, Vec2f(0.4f, 0.046f));

	auto text2 = UI::Text::createText("This is a simple output example", "Engine_Assets/fonts/DroidSans.ttf", 12, Vec2i(450, 50), Color{ 255,255,255, 255 });
	text2->Physicalize(1.f, 1.f, Physics2D::BodyType::DYNAMIC);

	using namespace Physics2D;
	auto Ground = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::STATIC,
		BodyShape::BOX, BodyParams{ 1,1 },
		Vec2f{0.f, (float)(this->GetScreenHeight()+ 464 / PTM)-150.f },
		std::vector<Vec2f>{Vec2f((float)this->GetScreenWeight(), 464/ PTM)}
	);*/
	/*
	body = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::DYNAMIC,
		BodyShape::BOX, BodyParams{1.f,0.1f},
		Vec2f{ 50 + obj->getSize().x/PTM, 100+obj->getSize().y/PTM },
		std::vector<Vec2f>{Vec2f(obj->getSize().x/PTM, obj->getSize().y/PTM)}
	);

	body2 = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::DYNAMIC,
		BodyShape::BOX, BodyParams{ 1.f,0.1f },
		Vec2f{ 650 + anim->getSize().x/PTM, 75 + anim->getSize().y/PTM },
		std::vector<Vec2f>{Vec2f((anim->getSize().x-100)/PTM, (anim->getSize().y-10)/PTM)}
	);
	// attaching testing
	obj->attachTo(body, Vec2f(-0.5f, -0.5f));
	anim->attachTo(body2, Vec2f(-0.5f, -0.5f));

	//Phy simulation test
	body2->SetLinearVelocity(b2Vec2(-35.f, 0.f));
	body->SetLinearVelocity(b2Vec2(35.f, 0.f));
	body2->SetAngularDamping(6.f);
	*/
	//EVENT TESTING!!
	EventManager::GetEventManager()->addEventHandler<Events::ON_KEYBOARD_INPUT>(CALLBACK_2(Game::On_Input, this));
	/*EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_START>(CALLBACK_1(Game::OnCollision, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_END>(CALLBACK_1(Game::OnCollisionEnd, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_CLICK>(CALLBACK_3(Game::OnClick, this));*/
	//EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_MOVE>(CALLBACK_1(Game::OnMouseMove, this));
};

void Game::On_Engine_Render(float dt)
{
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
	}
	anim->setPosition(Vec2i(anim->getPosition().x+1, anim->getPosition().y+0));*/
	//obj->setPosition(Vec2i(obj->getPosition().x+1, obj->getPosition().y+0));

};

void Game::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{ 
	/*if (p_KeyState == SDL_KEYDOWN) {
		if (p_Key == SDLK_RIGHT) {
			anim->setPosition(Vec2i(anim->getPosition().x + 10, anim->getPosition().y + 0));
			anim->Flip(FLIPTYPE::NONE);
			//text->updateText("HELLO THERE YOU WANT A COFFEE ?");
			//text->setColor(Color(255, 0, 0, 255));
			//text->Rotate(45);
			Console::getConsole()->outputConsole("This is another output !");
		}
		else if (p_Key == SDLK_LEFT) {
			anim->setPosition(Vec2i(anim->getPosition().x - 10, anim->getPosition().y + 0));
			anim->Flip(FLIPTYPE::HORIZONTAL);
			//text->updateText("Yes please!");
			//text->setColor(Color(0, 0, 255, 255));
			Console::getConsole()->outputConsole("This is another output :D");
		}
		else if (p_Key == SDLK_c) {
			if (!f) {
				FREE(map);
				f = true;
			}
			//text->updateText("Sure here you go mate!");
			//text->setColor(Color(0, 255, 0, 255));
			Console::getConsole()->outputConsole("Maybe the last output :P");
		}
	}*/
	if (p_Key == SDLK_DOLLAR) {
		if (p_KeyState == SDL_KEYDOWN) {
			Console::getConsole()->Activate(!Console::getConsole()->IsActive());
		}
	}
/*	if (p_Key == SDLK_a && p_KeyState == SDL_KEYDOWN)
	{
		
		editBox->ActivateEditing(true);
	}
	if (p_Key == SDLK_z && p_KeyState == SDL_KEYDOWN)
	{
		editBox->ActivateEditing(false);
	}*/
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
