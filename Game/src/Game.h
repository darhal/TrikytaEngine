#pragma once
#include "core/Common/EngineInstance.h"
#include "GUI/GUIManager.hpp"

struct ENGINE_CONFIG;

class Game : public EngineInstance
{
public:
	Game(const ENGINE_CONFIG& p_Config) :
		EngineInstance(p_Config), GUI_Manager(nullptr)
	{}

	void On_Engine_Pre_Init() override;
	void On_Engine_Init() override;
	void On_Engine_Render(float) override;
	void On_Engine_Quit() override;
private:
	GUIManager* GUI_Manager;
};