#pragma once
#include "core/Common/EngineInstance.h"

class Game : public EngineInstance
{
public:
	Game(std::string p_Wnd_Name, const int p_WindowWidth, const int p_WindowHeight) :
		EngineInstance(p_Wnd_Name, p_WindowWidth, p_WindowHeight)
	{}

	void On_Engine_Pre_Init() override;
	void On_Engine_Init() override;
	void On_Engine_Render(float) override;
	void On_Engine_Quit() override;

	void On_Input(int, unsigned int);
	void OnCollision(class b2Contact*);
	void OnCollisionEnd(b2Contact*);
	void OnClick(unsigned int, int, Vec2i);
	void OnMouseMove(Vec2i);

	class Sprite* obj;
	class Animation* anim;
	class UI::Text* text;
	class Physics2D::PhysicsBody* body;
	class Physics2D::PhysicsBody* body2;
	class TiledMap* map;
};