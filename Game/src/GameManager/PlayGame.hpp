#pragma once
#include <core/Physics/PhysicsEngine.h>
#include <SDL/SDL.h>
#include "../GUI/InGameMenu.hpp"

class PlayGame
{
public:
	PlayGame(class GameManager*);
	PlayGame() : m_GameManager(nullptr){};
	~PlayGame();
	void Init(class LoadingMenu*);

	void Collision(class b2Contact*);
	void Render(float);
	void On_Input(SDL_Keycode p_Key, unsigned int p_KeyState);
	std::vector<class Object*> m_ObjectsCreated;
private:
	class GameManager* m_GameManager;
	InGameMenu* m_InGameMainMenu;
	class TiledMap* map;
	class AnimationSet* anim;
	class Camera* cam;
	class Sprite* m_Background;
	Physics2D::PhysicsBody* body;
	bool moveleft;
	int compteurCoeur;
	int compteurPiece;
	int compteurKey;
	Sprite* coeurs[4];
	UI::Text* pieceScore;
	UI::Text* keyScore;
	std::vector<Physics2D::PhysicsBody*> mechantBody;
	bool anti_spam;
protected:
};