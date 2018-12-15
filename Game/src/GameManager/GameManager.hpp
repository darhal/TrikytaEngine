#pragma once

class GameManager
{
public:
	GameManager(class GUIManager*);
	void InitGame();
private:
	class GUIManager* m_GUIManager;
};