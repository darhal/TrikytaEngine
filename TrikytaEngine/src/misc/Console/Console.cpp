#include "core/InputManager/InputManager.h"
#include "core/Common/TrikytaEngine.h"
#include "ConsoleCommandField.h"
#include "core/Common/Utility.h"
#include "ConsoleCommands.h"
#include "ConsoleText.h"
#include "misc/Font.h"
#include "Console.h"


Console* Console::_Console = nullptr;
class Font* Console::m_Font = nullptr;

void Console::InitConsole()
{
	if (_Console == nullptr) {
		m_Font = Font::createOrGetFont(FONT_PATH, CONSOLE_TEXT_SIZE); // destroy this at then end
		_Console = new Console();
	}
	_Console->initConsoleCommandField();
	ConsoleCommands ccmd;
	ccmd.initCommands();
}

Console* Console::getConsole()
{
	return _Console;
}

void Console::AddCommandHandler(const std::string& cmd, std::function<void(const std::vector<std::string>&)> p_Func)
{
	Console::getConsole()->m_CommandField->AddCommandHandler(cmd, p_Func);
}

Console::Console() : m_isActive(false)
{
	m_StartPos = (int)ENGINE->GetScreenHeight() / 6;
	m_Output.reserve(MAX_CONSOLE_OUPUT);
	m_ConsoleBoundries = SDL_Rect{ (int)START_POS_X, 0, (int)(ENGINE->GetScreenWidth() - START_POS_X * 2), m_StartPos };
	m_ConsoleLineBoundries = SDL_Rect{ (int)START_POS_X-1, -1, (int)(ENGINE->GetScreenWidth() - START_POS_X * 2)+2, m_StartPos+2 };
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
	m_CommandField = ConsoleCommandField::createConsoleCommandField(">", Vec2i(START_POS_X+15, m_StartPos), { 255,255,255,255});
	m_CommandField->setPosition(Vec2i(START_POS_X+15, m_StartPos - m_CommandField->getTextSize().y));
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
	SDL_SetRenderDrawBlendMode(ENGINE->getRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 200);
	SDL_RenderFillRect(ENGINE->getRenderer(), &m_ConsoleBoundries);
	SDL_RenderDrawRect(ENGINE->getRenderer(), &m_ConsoleBoundries);
	SDL_RenderDrawRect(ENGINE->getRenderer(), &m_ConsoleLineBoundries);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	m_CommandField->render(dt);
	m_CommandField->renderConsoleText();
	for (auto msg : m_Output) {
		msg->renderConsoleText();
	}
	SDL_SetRenderDrawBlendMode(ENGINE->getRenderer(), SDL_BLENDMODE_NONE);
}

void Console::Activate(bool p_isActive)
{
	m_isActive = p_isActive;
	m_CommandField->setVisible(m_isActive);
}

bool Console::IsActive() const
{
	return m_isActive;
}