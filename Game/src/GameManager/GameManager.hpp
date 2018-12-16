#pragma once

class GameManager
{
public:
	GameManager(class GUIManager*, class LoadingMenu*);
	void InitGame();
	void BeginPlay();
private:
	class GUIManager* m_GUIManager;
	class LoadingMenu* m_LoadingMenu;
	class TiledMap* map;
	class AnimationSet* anim;
	class Camera* cam;
};