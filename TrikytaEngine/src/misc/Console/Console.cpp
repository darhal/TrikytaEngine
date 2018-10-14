#include "Console.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Common/Utility.h"
#include "ConsoleText.h"
#include "misc/Font.h"
#include "core/InputManager/InputManager.h"
#include <sstream>
#include <LStateManager/LStateManager.h>

Console* Console::_Console = nullptr;
class Font* Console::m_Font = nullptr;

void Console::InitConsole()
{
	if (_Console == nullptr) {
		m_Font = Font::createOrGetFont(FONT_PATH, CONSOLE_TEXT_SIZE); // destroy this at then end
		_Console = new Console();
	}
	_Console->initConsoleCommandField();
}

Console* Console::getConsole()
{
	return _Console;
}

Console::Console() : m_isActive(false)
{
	m_StartPos = (int)ENGINE->GetScreenHeight() / 6;
	m_Output.reserve(MAX_CONSOLE_OUPUT);
	m_ConsoleBoundries = SDL_Rect{ (int)START_POS_X, 0, (int)(ENGINE->GetScreenWeight() - START_POS_X * 2), m_StartPos };
	
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

void Console::outputConsole(std::string p_Text, MESSAGE_TYPE p_Type)
{
	if (p_Type == MESSAGE_TYPE::ERROR) {
		Console::outputConsole(std::string("[ERROR]: ")+p_Text, Color{255, 80, 80, 255}, false);
	}else if (p_Type == MESSAGE_TYPE::INFO) {
		Console::outputConsole(std::string("[INFO]: ")+p_Text, Color{0, 204, 0, 255}, false);
	}else if (p_Type == MESSAGE_TYPE::WARNING) {
		Console::outputConsole(std::string("[WARNING]: ")+p_Text, Color{255, 204, 0, 255}, false);
	}
}

void Console::outputConsole(std::string p_Text, Color p_Color, bool p_ShowTime)
{
	for (auto msg : m_Output) {
		msg->setPosition(Vec2i(msg->getPosition().x, msg->getPosition().y - m_Output.back()->getSize().y));
	}
	ConsoleText* output;
	output = ConsoleText::createText("["+std::string(Utility::getDateNow())+"] " + p_Text, Vec2i(START_POS_X+15, m_StartPos), p_Color);
	output->setPosition(Vec2i(START_POS_X+15, m_StartPos - output->getSize().y*2));
	LogTerminalFromConsole(("[CONSOLE]"+p_Text).c_str());
	m_Output.emplace_back(output);
	removeConsoleMessage();
}

void Console::initConsoleCommandField()
{
	m_CommandField = ConsoleText::createText(">", Vec2i(START_POS_X+15, m_StartPos), { 255,255,255,255});
	m_CommandField->setPosition(Vec2i(START_POS_X+15, m_StartPos - m_CommandField->getSize().y));
}

void Console::removeConsoleMessage()
{
	if (m_Output.front()->getPosition().y < 0) {
		FREE(m_Output.front());
		m_Output.erase(m_Output.begin());
	}
}

void Console::Draw(float dt)
{
	if (!m_isActive) return;
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 185);
	SDL_RenderFillRect(ENGINE->getRenderer(), &m_ConsoleBoundries);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	m_CommandField->renderConsoleText();
	for (auto msg : m_Output) {
		msg->renderConsoleText();
	}
}

void Console::ProcessConsole(SDL_Event& e)
{
	if (m_isActive) {
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_RETURN && m_CommandField->m_Text.length() > 1)//Handle backspace
			{
				CommandExec(m_CommandField->m_Text);
				m_CommandField->m_Text = ">";
				m_CommandField->updateTextHelper();
			}else if (e.key.keysym.sym == SDLK_BACKSPACE && m_CommandField->m_Text.length() > 1)//Handle backspace
			{
				m_CommandField->m_Text.pop_back();
				m_CommandField->updateTextHelper();
			}else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {//Handle copy
				SDL_SetClipboardText(m_CommandField->m_Text.c_str());
			}
			else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) { //Handle paste
				m_CommandField->m_Text = SDL_GetClipboardText();
				m_CommandField->updateTextHelper();
			}
		}else if (e.type == SDL_TEXTINPUT) { //Special text input event
			  //Not copy or pasting
			if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
			{
				if (e.text.text[0] != '$'){
					//Append character
					m_CommandField->m_Text += e.text.text;
					m_CommandField->updateTextHelper();
				}
			}
		}else if (e.type == SDL_TEXTEDITING) {
			/*composition = event.edit.text;
			cursor = event.edit.start;
			selection_len = event.edit.length;*/
		}
	}
}

void Console::CommandExec(std::string& cmd)
{
	cmd.erase(cmd.begin()); // remove the '>'
	std::vector<std::string> args;
	std::string tempArg;
	std::istringstream scmd(cmd); 
	while (getline(scmd, tempArg, ' ')) {
		args.push_back(tempArg);
	}
	auto itr = std::find(m_Commands.begin(), m_Commands.end(), args.at(0));
	if (itr == m_Commands.end()){ // seach for cmd (index 0 is the cmd)
		LogConsole(LogError, "Command '%s' not found!", args.at(0).c_str());
		return;
	}
	args.erase(args.begin()); // delete the cmd keep only the args
	auto index = std::distance(m_Commands.begin(), itr);
	m_CmdFunctions.at(index)(args); // Function call!
}

void Console::Activate(bool p_isActive)
{

	InputManager::getInputManager()->ActivateInput(p_isActive);
	m_isActive = p_isActive;
}

bool Console::IsActive() const
{
	return m_isActive;
}