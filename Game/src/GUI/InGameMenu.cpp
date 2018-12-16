#include "InGameMenu.hpp"
#include "GUIManager.hpp"
#include <UI/UIButton.hpp>
#include <UI/UIWidget.hpp>
#include <UI/UIManager.h>
#include <UI/UIProgressbar.hpp>
#include <UI/UIImage.hpp>
#include <UI/UIEditBox.h>
#include <UI/UIText.h>
#include <core/Common/TrikytaEngine.h>
#include <core/Events/EventManager.h>
#include "GUIManager.hpp"
#include <sound/Music.hpp>
#include <sound/SoundEffect.hpp>
#include <core/Utility/TimerManager.h>

InGameMenu::InGameMenu(GUIManager* m, GameManager* gm) : BaseMenu(m), m_GameManager(gm)
{
	InGameMenu::BuildInGameMenu();
}

InGameMenu::~InGameMenu()
{
	m_Widget->setVisible(false);
	FREE(m_Widget);
}

void InGameMenu::BuildInGameMenu()
{
	using namespace UI;
	Vec2i m_ButtonSize = Vec2i(250, 50);
	int spacement = 10;
	m_Widget = new Widget("Main Menu", Vec2i(50, 50), Vec2i(300, 300));
	Font* font = Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 24);
	font->setTextStyle(Font::Style::BOLD);
	m_Widget->Configure(WidgetParam{ font , {66, 68, 68, 200}, {232, 163, 25, 255}, {232, 62, 73, 200}, {0,0,0,255} });
	m_Widget->setPos(Vec2i((ENGINE->GetScreenWidth() - m_Widget->getSize().x) / 2, int(ENGINE->GetScreenHeight() - m_Widget->getSize().y)/2));
	const std::string buttonsName[4] = { "Resume", "Mute/Unmute Music", "Leave level", "Exsit Game" };
	for (int i = 0; i < 4; i++) {
		Button* button = new Button(buttonsName[i], Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 22), Vec2i((m_Widget->getSize().x- m_ButtonSize.x)/2, m_ButtonSize.y + (i)*(spacement + m_ButtonSize.y)), m_ButtonSize, Color{255,255,255,200});
		m_InGameMenu.emplace_back(button);
		m_Widget->AddElement(button);
		button->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click)
		{
			if (is_click) {
				GUI_Manager->m_ClickEffect->Play(0);
			}
			m_CurrentButton = buttonsName[i];
			OnRootButtonClick(is_click);
		});
	}
	m_Widget->setVisible(false);
}

void InGameMenu::ShowMenu(bool v)
{
	m_Widget->setVisible(v);
	v ? GUI_Manager->m_MenuType = INGAME_MENU : GUI_Manager->m_MenuType = NO_MENU;
	v ? GUI_Manager->m_CurrentMenu = this : GUI_Manager->m_CurrentMenu = NULL;
}

void InGameMenu::OnRootButtonClick(bool is_click)
{
	if (is_click) { return; }
	if (m_CurrentButton == "Resume") {
		TimerManager::CreateTimer([this]() {ShowMenu(false); }, 100, 1);
	}else if (m_CurrentButton == "Mute/Unmute Music") {
		GUI_Manager->MuteMusic(GUI_Manager->m_MainMusic->IsPlaying());
	}else if (m_CurrentButton == "Exsit Game") {
		TimerManager::CreateTimer([=]() {
			if (dynamic_cast<InGameMenu*>(GUI_Manager->m_CurrentMenu) != nullptr)
			{
				auto ptr = dynamic_cast<InGameMenu*>(GUI_Manager->m_CurrentMenu)->m_GameManager;
				FREE(ptr);
			}
			FREE(GUI_Manager);
			ENGINE->setEngineState(false);
		}, 150, 1, true);
	}else if (m_CurrentButton == "Leave level") {
		GUI_Manager->OnLeaveMenu(INGAME_MENU, MAIN_MENU);
	}
}