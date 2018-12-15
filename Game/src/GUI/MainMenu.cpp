#include "MainMenu.hpp"
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

MainMenu::MainMenu(GUIManager* GUI_Manager) : m_IsHover(false), BaseMenu(GUI_Manager)
{
	BuildMainMenu();
	m_HoverBoxText = UI::Text::createText("N/A", Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 16), Vec2i(0, 0), Color{255, 255, 255}, false);
}

void MainMenu::BuildMainMenu()
{
	using namespace UI;
	int spacement = 20;
	m_ClickEffect = new SoundEffect("assets/gui/click_effect.wav");
	Vec2i m_ButtonSize = Vec2i(int(214 / 1.5f), int(215 / 1.5f));
	m_Widget = new Widget("Main Menu", Vec2i(50, 50), Vec2i((m_ButtonSize.x + spacement)*5, 220));
	Font* font = Font::createOrGetFont("Engine_Assets/fonts/DroidSans.ttf", 24);
	font->setTextStyle(Font::Style::BOLD);
	m_Widget->Configure(WidgetParam{ font , {200, 200, 200, 0}, {232, 163, 25, 255}, {232, 62, 73, 200}, {0,0,0,255} });
	m_Widget->setPos(Vec2i((ENGINE->GetScreenWidth()- m_Widget->getSize().x)/2, int(ENGINE->GetScreenHeight()- m_Widget->getSize().y*1.3f)));
	const std::string buttonsName[5] = { "play.png", "menu.png", "settings.png", "music_off.png", "about.png" };
	for (int i=0;i<5;i++){
		Image* button = new Image("assets/gui/" + buttonsName[i], Vec2i(10+(i)*(spacement+ m_ButtonSize.x), int(m_Widget->getSize().y/4)), m_ButtonSize);
		m_MainMenuButtons.emplace_back(button);
		m_Widget->AddElement(button);
		button->addEventHandler<ON_UI_HOVER>([=](const Vec2i&, bool is_hover)
		{
			if (is_hover) {
				button->SetColor(Color{ 249, 141, 0 });
				button->setPos(button->getPos() + Vec2i(0, -5));
				m_HoverText = buttonsInfo[i];
				m_HoverBoxText->updateText(m_HoverText);
				m_IsHover = true;
			}else {
				button->SetColor(Color{ 255, 255, 255 });
				button->setPos(button->getPos() - Vec2i(0, -5));
				m_IsHover = false;
				m_HoverText = "";
			}
		});
		button->addEventHandler<ON_UI_CLICK>([=](const Vec2i&, bool is_click)
		{
			if (is_click) {
				button->SetColor(Color{ 0, 255, 208 });
				m_ClickEffect->Play(0);
			}else {
				button->SetColor(Color{ 249, 141, 0 });
			}
			OnRootButtonClick(is_click);
		});
	}
	AddEventHandler(ON_MOUSE_MOVE, CALLBACK_1(MainMenu::OnMouseMove, this));
	AddEventHandler(ON_ENGINE_RENDER, CALLBACK_1(MainMenu::Refresh, this));
}

void MainMenu::Refresh(float dt)
{
	if (m_IsHover) {
		auto r = ENGINE->getRenderer();
		int padding = 1;
		int advValue = 13;
		auto widgetBounderies = SDL_Rect{ advValue +m_MousePos.x+padding, m_MousePos.y+padding, int(m_HoverBoxText->getSize().x*1.2f) - padding * 2, int(m_HoverBoxText->getSize().y*1.2f) - padding * 2 };
		SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 180);
		SDL_RenderFillRect(r, &widgetBounderies);
		SDL_SetRenderDrawColor(r, 255, 255, 255, 180);
		for (int i = padding; i >= 0; i--) {
			widgetBounderies = SDL_Rect{ widgetBounderies.x - 1, widgetBounderies.y - 1, widgetBounderies.w + 2, widgetBounderies.h + 2 };
			SDL_RenderDrawRect(r, &widgetBounderies);
		}
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		m_HoverBoxText->setPosition(Vec2i(m_MousePos.x + advValue, m_MousePos.y)+(Vec2i(int(m_HoverBoxText->getSize().x*1.2f), int(m_HoverBoxText->getSize().y*1.2f)) - m_HoverBoxText->getSize()) / 2);
		m_HoverBoxText->render(dt);
		SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
	}
}

void MainMenu::OnMouseMove(const Vec2i& pos)
{
	if (m_IsHover) {
		m_MousePos = pos;
	}
}

void MainMenu::OnRootButtonClick(bool is_click)
{
	if (is_click) { return; }
	if (m_HoverText == "Mute music" || m_HoverText == "Unmute music") {
		bool isPlaying = GUI_Manager->getMainMusic()->IsPlaying();
		if (isPlaying) {
			m_MainMenuButtons.at(3)->ChangeTexture("assets/gui/music_on.png");
			buttonsInfo[3] = "Unmute music";
			m_HoverText = buttonsInfo[3];
			m_HoverBoxText->updateText(m_HoverText);
		}else{
			m_MainMenuButtons.at(3)->ChangeTexture("assets/gui/music_off.png");
			buttonsInfo[3] = "Mute music";
			m_HoverText = buttonsInfo[3];
			m_HoverBoxText->updateText(m_HoverText);
		}
		GUI_Manager->MuteMusic(isPlaying);
	}else if (m_HoverText == "Play") {
		GUI_Manager->OnLeaveMenu(MAIN_MENU, LOADING_MENU);
	}
}

MainMenu::~MainMenu()
{
	ClearAllEventHandlers(ON_MOUSE_MOVE);
	ClearAllEventHandlers(ON_ENGINE_RENDER);
	m_Widget->setVisible(false);
	m_HoverBoxText->setVisible(false);
	m_IsHover = false;
	for(auto ui : m_Widget->GetElements()){
		FREE(ui);
	}
	FREE(m_HoverBoxText);
	FREE(m_Widget);
	FREE(m_ClickEffect);
}
