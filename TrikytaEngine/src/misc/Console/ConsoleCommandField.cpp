#include "core/InputManager/InputManager.h"
#include <LStateManager/LStateManager.h>
#include "ConsoleCommandField.h"
#include "core/Common/Color.h"
#include "UI/UIManager.h"
#include <sstream>


ConsoleCommandField::ConsoleCommandField(std::string p_Text, Vec2i p_Pos, Color p_Color)
	: ConsoleText(p_Text, p_Pos, p_Color)
{
	// adding a command restart!
	m_Commands.push_back("restart");
	m_CmdFunctions.push_back
	(
		[](std::vector<std::string> args)
		{
			LogConsole(LogInfo, "Restarting script %s ...", args.at(0).c_str());
			LuaEngine::LStateManager::GetLStateManager()->RestertScript(args.at(0));
		}
	);
}

void ConsoleCommandField::ProcessEventHelper(SDL_Event& e)
{
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
		auto console_rect = Console::getConsole()->getConsoleBoundries();
		Vec2i maxPos = Vec2i(console_rect.x+ console_rect.w, console_rect.y+ console_rect.h);
		auto mc = Vec2i(e.button.x, e.button.y);
		if (UI::Base::IsInBox(mc, Vec2i(console_rect.x, console_rect.y), maxPos))
		{
			if (!m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus);
			}
			m_IsFocus = true;
			OnUIClick(mc - getPos(), e.type == SDL_MOUSEBUTTONDOWN ? true : false);
		}else {
			if (m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus);
			}
			m_IsFocus = false;
		}
	}
}

void ConsoleCommandField::PorcessEvents(SDL_Event& e)
{
	ConsoleCommandField::ProcessEventHelper(e);
	if (isFocus()) {
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_RETURN && m_Text.length() > 1)//Handle backspace
			{
				CommandExec(m_Text);
				m_Text = ">";
				updateTextHelper();
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getSize());
			}
			else if (e.key.keysym.sym == SDLK_BACKSPACE && m_Text.length() > 1)//Handle backspace
			{
				m_Text.pop_back();
				updateTextHelper();
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getSize());
			}
			else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {//Handle copy
				SDL_SetClipboardText(m_Text.c_str());
			}
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { //Handle paste
				m_Text = SDL_GetClipboardText();
				updateTextHelper();
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getSize());
			}
		}
		else if (e.type == SDL_TEXTINPUT) { //Special text input event
			 //Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				if (e.text.text[0] != '$') {
					m_Text += e.text.text;
					updateTextHelper();
					InputManager::getInputManager()->setCurosrPosition(getPosition(), getSize());
				}
			}
		}
		else if (e.type == SDL_TEXTEDITING) {
			/*composition = event.edit.text;
			cursor = event.edit.start;
			selection_len = event.edit.length;*/
		}
	}
}

void ConsoleCommandField::CommandExec(std::string& cmd)
{
	cmd.erase(cmd.begin()); // remove the '>'
	std::vector<std::string> args;
	std::string tempArg;
	std::istringstream scmd(cmd);
	while (getline(scmd, tempArg, ' ')) {
		args.push_back(tempArg);
	}
	auto itr = std::find(m_Commands.begin(), m_Commands.end(), args.at(0));
	if (itr == m_Commands.end()) { // seach for cmd (index 0 is the cmd)
		LogConsole(LogError, "Command '%s' not found!", args.at(0).c_str());
		return;
	}
	args.erase(args.begin()); // delete the cmd keep only the args
	auto index = std::distance(m_Commands.begin(), itr);
	m_CmdFunctions.at(index)(args); // Function call!
}

void ConsoleCommandField::OnUIClick(Vec2i, bool)
{

};

void ConsoleCommandField::render(float dt)
{
	InputManager::getInputManager()->DrawCursor(dt);
}

void ConsoleCommandField::OnUIFocus(bool isFocus)
{
	UI::EditBoxBase::OnUIFocus(isFocus);
	if (isFocus) {
		setColor(Color{255,255,255,255});
	} else {
		setColor(Color{205,205,205,200});
	}
};

Vec2i ConsoleCommandField::getPos()
{
	return ConsoleText::getPosition();
};

Vec2i ConsoleCommandField::getSize()
{
	return ConsoleText::getSize();
};

ConsoleCommandField::~ConsoleCommandField()
{

}

