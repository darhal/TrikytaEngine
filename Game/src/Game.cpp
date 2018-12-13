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
#include <core/Drawable/AnimationSet.hpp>
#include <UI/UIButton.hpp>
#include <UI/UIWidget.hpp>
#include <UI/UIManager.h>
#include <UI/UIProgressbar.hpp>
#include <UI/UIImage.hpp>
#include <sound/SoundEffect.hpp>
#include <sound/Music.hpp>

//UI::EditBox* editBox;
Camera* cam;
bool anti_spam = false;
AnimationSet* aa;

void Game::On_Engine_Pre_Init()
{
	LogTerminal("Trikyta Engine Pre-initializing. ");
};

void Game::On_Engine_Init()
{
	auto menu_background = Sprite::Create("assets/gui/background1.png", Vec2i(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()), Vec2i(0, 0));
	Music* music = new Music("assets/gui/main_theme.mp3");
	music->Play(-1);
	auto level_selector_btn = new UI::Image("assets/gui/level_selector1.png", Vec2i(200, 200), Vec2i(64, 64));
	level_selector_btn->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click) {is_click ? level_selector_btn->ChangeTexture("assets/gui/level_selector3.png") : level_selector_btn->ChangeTexture("assets/gui/level_selector2.png"); });
	level_selector_btn->addEventHandler<ON_UI_HOVER>([=](const Vec2i&, bool is_hover) {is_hover ? level_selector_btn->ChangeTexture("assets/gui/level_selector2.png") : level_selector_btn->ChangeTexture("assets/gui/level_selector1.png"); });
	auto settings_btn = new UI::Image("assets/gui/settings1.png", Vec2i(200+74+64, 200), Vec2i(64, 64));
	settings_btn->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click) {is_click ? settings_btn->ChangeTexture("assets/gui/settings3.png") : settings_btn->ChangeTexture("assets/gui/settings2.png"); });
	settings_btn->addEventHandler<ON_UI_HOVER>([=](const Vec2i&, bool is_hover) {is_hover ? settings_btn->ChangeTexture("assets/gui/settings2.png") : settings_btn->ChangeTexture("assets/gui/settings1.png"); });
	auto mute_btn = new UI::Image("assets/gui/mute1.png", Vec2i(200 + (74 + 64) * 2, 200), Vec2i(64, 64));
	auto unmute_btn = new UI::Image("assets/gui/unmute1.png", Vec2i(200 + (74 + 64) * 2, 200), Vec2i(64, 64));
	unmute_btn->setVisible(false);
	mute_btn->addEventHandler<ON_UI_HOVER>([=](const Vec2i&, bool is_hover) {is_hover ? mute_btn->ChangeTexture("assets/gui/mute2.png") : mute_btn->ChangeTexture("assets/gui/mute1.png"); });
	mute_btn->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click) {
		if (is_click) {
			mute_btn->ChangeTexture("assets/gui/mute3.png");
		}else {
			if (music->IsPlaying()) {
				music->Stop(FADE, 40);
				TimerManager::CreateTimer([=]() {unmute_btn->setVisible(true); mute_btn->setVisible(false); }, 50, 1, true);
				mute_btn->ChangeTexture("assets/gui/mute1.png");
			}
		}
	});
	unmute_btn->addEventHandler<ON_UI_HOVER>([=](const Vec2i&, bool is_hover) {is_hover ? unmute_btn->ChangeTexture("assets/gui/unmute2.png") : unmute_btn->ChangeTexture("assets/gui/unmute1.png"); });
	unmute_btn->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click) {
		if (is_click) {
			unmute_btn->ChangeTexture("assets/gui/unmute3.png");
		}else {
			if (music->IsStopped()) {
				music->Play(-1);
				TimerManager::CreateTimer([=]() {unmute_btn->setVisible(false); mute_btn->setVisible(true); }, 50, 1, true);
				unmute_btn->ChangeTexture("assets/gui/unmute1.png");
			}
		}
	});
	


	/*cam = Camera::CreateCamera();
	map = TiledMap::Create("assets/example/maps/map/map.tmx");
	cam->addObjectToCamera(map);
	anim = AnimationSet::Create("assets/player.png", "assets/player.txt", Vec2i(256/7, 217/7), Vec2i(ENGINE->GetScreenWidth() / 2, (ENGINE->GetScreenHeight() / 2)-500), 0.03f);
	body = anim->Physicalize(Physics2D::BodyParams{ 1.f, 0.2f }, Physics2D::BodyType::DYNAMIC, Physics2D::BodyShape::CIRCLE, Vec2f(0.35f, 0.013f));
	cam->addObjectToCamera(anim);
	anim->ToggleRotationAttachement(false);
	body->SetAngularDamping(1000.f);
	anim->setAnimation("Idle");

	//Sound tests:
	SoundEffect* clickEffect = new SoundEffect("assets/medium.wav");
	Music* music = new Music("assets/main_theme.mp3");
	music->Play(0);
	music->addEventHandler<ON_SOUND_FINISH>([]() {LogTerminal("Sound finished!"); });

	//UI TESTS:
	using namespace UI;
	auto editBox = EditBox::createEditBox("Enter your name", "Engine_Assets/fonts/DroidSans.ttf", 16,
		Vec2i(10, 50), Vec2i(25*8, 30),Color{ 255,255,255, 255 });
	auto btn = Button::createButton("Click Me", Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 16), Vec2i(10, 100), Vec2i(25 * 5, 30), Color{ 255,255,255, 255 });
	Widget* widget = new Widget("This is test", Vec2i(500, 250), Vec2i(300, 300));
	Font* font = Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 18);
	font->setTextStyle(Font::Style::BOLD);
	widget->Configure(WidgetParam{ font , {200, 200, 200, 220}, {232, 163, 25, 255}, {232, 62, 73, 200}, {0,0,0,255} });
	widget->AddElement(btn);
	widget->AddElement(editBox);
	auto pb = new Progressbar(Vec2i(10, 150), Vec2i(25 * 8, 30), Color(0, 0, 0, 200), Color(98, 204, 239, 255), "Loading...", Color{0,0,0,255}, Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 18));
	widget->AddElement(pb);
	editBox->addEventHandler<ON_EDITBOX_CHANGE>([=](const char* c) {pb->setProgress(pb->getProgress() - 5, Color{ 0, 174, 255, 200 }, Color{ 183, 232, 255, 200 }, 5); });
	btn->addEventHandler<ON_UI_CLICK>([=](const Vec2i& pos, bool is_down)
		{
			if (is_down) {
				LogTerminal("------------------------");
			}
		}
	);

	TimerManager::CreateTimer([=]() {pb->setProgress(pb->getProgress() + 5, Color{ 252, 0, 46, 200 }, Color{ 252, 103, 130, 200 }, 5); }, 1000, 20, true);
	
	auto image = new Image("assets/button/play.png", Vec2i(10, 200), Vec2i(701/8, 701/8));
	widget->AddElement(image);
	image->addEventHandler<ON_UI_CLICK>([=](const Vec2i& pos, bool is_click) {
		if (is_click) {
			clickEffect->Play(0);
			image->ChangeTexture("assets/button/play_click.png");
		}else {
			image->ChangeTexture("assets/button/play_hover.png");
		}
	});
	image->addEventHandler<ON_UI_HOVER>([=](const Vec2i& pos, bool is_hover) {
		if (is_hover) {
			image->ChangeTexture("assets/button/play_hover.png");
		}
		else {
			image->ChangeTexture("assets/button/play.png");
		}
	});

	//EVENT TESTING!!
	//EventManager::GetEventManager()->addEventHandler<Events::ON_KEYBOARD_INPUT>(CALLBACK_2(Game::On_Input, this));
	EventManager::GetEventManager()->addEventHandler<ON_COLLISION_START>(CALLBACK_1(Game::OnCollision, this));
	AddEventHandler(ON_KEYBOARD_INPUT, CALLBACK_2(Game::On_Input, this));
	AddConsoleCommand("setanim", [=](const std::vector<std::string>& args) {std::string anim_name = args.at(0); anim->setAnimation(anim_name); });
	AddConsoleCommand("setvol", [=](const std::vector<std::string>& args)
	{
		std::string vol = args.at(0);
		int v = std::stoi(vol.c_str());
		music->setVolume(v);
		LogConsole(LogInfo, "Setting volume to %d", music->getVolume());
	});
	AddConsoleCommand("pause", [=](const std::vector<std::string>& args)
	{
		music->Pause();
		LogConsole(LogInfo, "Music paused");
	});
	AddConsoleCommand("resume", [=](const std::vector<std::string>& args)
	{
		music->Resume();
		LogConsole(LogInfo, "Music playing");
	});
	AddConsoleCommand("rewind", [=](const std::vector<std::string>& args)
	{
		music->Rewind();
		LogConsole(LogInfo, "Music rewinded");
	});
	AddConsoleCommand("setposition", [=](const std::vector<std::string>& args)
	{
		std::string pos = args.at(0);
		double p = std::stod(pos.c_str());
		music->setPosition(p);
		LogConsole(LogInfo, "Set position to %f", p);
	});*/
	//AddEventHandler(ON_MOUSE_CLICK, CALLBACK_3(Game::OnClick, this));
	//AddEventHandler(ON_MOUSE_MOVE, CALLBACK_1(Game::OnMouseMove, this));
	/*EventManager::GetEventManager()->addEventHandler<Events::ON_COLLISION_END>(CALLBACK_1(Game::OnCollisionEnd, this));
	EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_CLICK>(CALLBACK_3(Game::OnClick, this));*/
	//EventManager::GetEventManager()->addEventHandler<Events::ON_MOUSE_MOVE>(CALLBACK_1(Game::OnMouseMove, this));
};


void Game::OnButtonClick(const Vec2i& pos, bool is_down)
{
	LogTerminal("Button clicked Pos=(%d, %d) bool = %d", pos.x, pos.y, is_down);
}

void Game::On_Engine_Render(float /*dt*/)
{
	// Offset the player quad by the camera position
	/*bool b = true;
	Vec2i pos = Vec2i((int)body->GetPosition().x, (int)body->GetPosition().y) - Vec2i(cam->getCameraSize().x/2, cam->getCameraSize().y/2);
	if (b)
		cam->setCameraPosition(pos);*/
};

void Game::On_Input(SDL_Keycode p_Key, unsigned int p_KeyState)
{
	if (p_KeyState == SDL_KEYDOWN) {
		if (p_Key == SDLK_RIGHT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::NONE);
			anim->setAnimation("Run");
		}else if (p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = -50.f;
			body->SetLinearVelocity(v);
			anim->Flip(FLIPTYPE::HORIZONTAL);
			anim->setAnimation("Run");
		}else if (p_Key == SDLK_UP) {
			//body->ApplyLinearImpulse(Vec2f(0, body->GetMass() * -1000.f), body->GetWorldCenter(), false);
			Vec2f v = body->GetLinearVelocity();
			v.y = -50.f;
			body->SetLinearVelocity(v);
			anim->setAnimation("Jump");
		}else if (p_Key == SDLK_DOWN) {
			//aa->setAnimation("Idle");
		}else if (p_Key == SDLK_c) {
			//aa->setAnimation("Attack");
		}
	}else if (p_KeyState == SDL_KEYUP) {
		if (p_Key == SDLK_RIGHT || p_Key == SDLK_LEFT) {
			Vec2f v = body->GetLinearVelocity();
			v.x = 0.f;
			body->SetLinearVelocity(v);
			anim->setAnimation("Idle");
		}else if (p_Key == SDLK_UP) {
			anim->setAnimation("Fall");
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
