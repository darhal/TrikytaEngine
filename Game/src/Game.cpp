#include "Game.h"
#include <iostream>
#include <UI/UIText.h>
#include <UI/UIManager.h>
#include <UI/UIImage.hpp>
#include <UI/UIEditBox.h>
#include <UI/UIButton.hpp>
#include <UI/UIWidget.hpp>
#include <sound/Music.hpp>
#include <core/Common/Vec2.h>
#include <UI/UIProgressbar.hpp>
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

void Game::On_Engine_Pre_Init()
{
	
};

void Game::On_Engine_Init()
{
	GUI_Manager = new GUIManager();
};

void Game::On_Engine_Render(float dt)
{
	GUI_Manager->Refresh(dt);
};

void Game::On_Engine_Quit()
{
	EngineInstance::On_Engine_Quit();
};

