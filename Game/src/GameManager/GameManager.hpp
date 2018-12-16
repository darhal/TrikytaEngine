#pragma once
#include "PlayGame.hpp"

class GameManager
{
public:
	GameManager(class GUIManager*, class LoadingMenu*);
	~GameManager();
	void InitGame();
	void BeginPlay();
private:
	class GUIManager* m_GUIManager;
	class LoadingMenu* m_LoadingMenu;
	class PlayGame* m_Game;
	friend PlayGame;
};