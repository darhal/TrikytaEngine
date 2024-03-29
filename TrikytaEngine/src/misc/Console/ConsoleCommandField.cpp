#include "core/InputManager/InputManager.h"
#include "ConsoleCommandField.h"
#include "core/Common/Color.h"
#include "UI/UIManager.h"
#include <sstream>
#include <algorithm>

ConsoleCommandField::ConsoleCommandField(std::string p_Text, const Vec2i& p_Pos, const Color& p_Color)
	: ConsoleText(p_Text, p_Pos, p_Color)
{
	//m_CmdHistroy.reserve(8);
	m_CmdHistorySelector = -1;
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
				OnUIFocus(!m_IsFocus, mc);
			}
			m_IsFocus = true;
			OnUIClick(mc - getPos(), e.type == SDL_MOUSEBUTTONDOWN ? true : false);
		}else {
			if (m_IsFocus && e.type == SDL_MOUSEBUTTONDOWN) {
				OnUIFocus(!m_IsFocus, mc);
			}
			m_IsFocus = false;
		}
	}
}

void ConsoleCommandField::PorcessEvents(SDL_Event& e)
{
	static unsigned int lastTimeStamp = 0; // This is to pervent double SDL_TEXTINPUT Detection in linux systems
	ConsoleCommandField::ProcessEventHelper(e);
	if (isFocus()) {
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_RETURN && m_Text.length() > 1)//Handle backspace
			{
				if (m_CmdHistroy.size() >= 8) { // max size
					m_CmdHistroy.pop_back();
				}
				if (std::find(m_CmdHistroy.begin(), m_CmdHistroy.end(), m_Text) == m_CmdHistroy.end()) {// if already not there
					m_CmdHistroy.emplace_front(m_Text); //add it
				}
				CommandExec(m_Text);
				m_Text = ">";
				m_CmdHistorySelector = -1;
				updateTextHelper();
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
			}else if (e.key.keysym.sym == SDLK_BACKSPACE && m_Text.length() > 1) { //Handle backspace
				m_Text.pop_back();
				updateTextHelper();
				m_CmdHistorySelector = -1;
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
			}else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) { //Handle copy
				m_Text.erase(m_Text.begin());
				SDL_SetClipboardText(m_Text.c_str());
				m_Text = ">" + m_Text;
			}
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { //Handle paste
				auto cachedText = m_Text;
				cachedText.erase(cachedText.begin());
				m_Text = ">"+ cachedText +std::string(SDL_GetClipboardText());
				updateTextHelper();
				InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
			}else if (e.key.keysym.sym == SDLK_UP) { //Handle paste
				if (m_CmdHistroy.size()) {
					m_CmdHistorySelector++;
					if ((unsigned)m_CmdHistorySelector > m_CmdHistroy.size() - 1) {
						m_CmdHistorySelector = m_CmdHistroy.size() - 1;
						return;
					}
					m_Text = m_CmdHistroy.at(m_CmdHistorySelector);
					updateTextHelper();
					InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
				}
			}else if (e.key.keysym.sym == SDLK_DOWN) { //Handle paste
				if (m_CmdHistroy.size()){
					m_CmdHistorySelector--;
					if (m_CmdHistorySelector < 0) {
						m_CmdHistorySelector = -1;
						m_Text = ">";
						updateTextHelper();
						InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
						return;
					}
					m_Text = m_CmdHistroy.at(m_CmdHistorySelector);
					updateTextHelper();
					InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
				}
			}
		}
		else if (e.type == SDL_TEXTINPUT && lastTimeStamp != e.text.timestamp) { //Special text input event
			 //Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				if (e.text.text[0] != '$') {
					m_Text += e.text.text;
					lastTimeStamp = e.text.timestamp;
					updateTextHelper();
					InputManager::getInputManager()->setCurosrPosition(getPosition(), getTextSize());
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

void ConsoleCommandField::CommandExec(std::string& p_CmdAndArgs)
{
	p_CmdAndArgs.erase(p_CmdAndArgs.begin()); // remove the '>'
	std::vector<std::string> args;
	std::string tempArg;
	std::istringstream scmd(p_CmdAndArgs);
	while (getline(scmd, tempArg, ' ')) {
		args.push_back(tempArg);
	}
	auto cmd = args.at(0);

	if (!m_CmdData.count(cmd)){ // seach for cmd (index 0 is the cmd)
		LogConsole(LogError, "Command '%s' not found!", cmd.c_str());
		return;
	}
	args.erase(args.begin()); // delete the cmd keep only the args
	m_CmdData[cmd].m_CmdFunction(args); //TODO: Find a solution for functions that take args but user enter cmd without args e.g : restart 

	//auto index = std::distance(m_Commands.begin(), itr);
	//m_CmdFunctions.at(index)(args); // Function call!
}

void ConsoleCommandField::AddCommandHandler(const std::string& p_Cmd, CmdFunc p_FuncHandlerconst)
{
	m_CmdData[p_Cmd].m_Command = p_Cmd;
	m_CmdData[p_Cmd].m_CmdFunction = p_FuncHandlerconst;
}

void ConsoleCommandField::OnUIClick(const Vec2i&, bool)
{

};

void ConsoleCommandField::render(float dt)
{
	InputManager::getInputManager()->DrawCursor(dt);
}

void ConsoleCommandField::OnUIFocus(bool isFocus, const Vec2i& mouseClick)
{
	UI::EditBoxBase::OnUIFocus(isFocus, mouseClick);
	if (isFocus) {
		setColor(Color{255,255,255,255});
	} else {
		setColor(Color{205,205,205,200});
	}
};

Vec2i ConsoleCommandField::getTextSize(){ return ConsoleText::getSize();};
Vec2i ConsoleCommandField::getTextPos(){return ConsoleText::getPosition();};
Vec2i ConsoleCommandField::getSize(){return ConsoleText::getSize();};
Vec2i ConsoleCommandField::getPos(){return ConsoleText::getPosition();};

ConsoleCommandField::~ConsoleCommandField()
{

}

