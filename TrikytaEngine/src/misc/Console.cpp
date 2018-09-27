#include "Console.h"
#include "UI/UIText.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Common/Utility.h"

Console* Console::_Console = nullptr;

void Console::InitConsole()
{
	if (_Console == nullptr) {
		_Console = new Console();
	}
}

Console* Console::getConsole()
{
	return _Console;
}

Console::Console():m_isActive(false)
{
	m_StartPos = ENGINE->GetScreenWeight() / 6;
	m_Output.reserve(MAX_CONSOLE_OUPUT);
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
	UI::Text* output;
	output = UI::Text::createText("["+std::string(Utility::getDateNow())+"] " + p_Text, FONT_PATH, CONSOLE_TEXT_SIZE, Vec2i(START_POS_X, m_StartPos), p_Color, false);
	Log(("[CONSOLE]"+p_Text).c_str());
	m_Output.emplace_back(output);
	removeConsoleMessage();
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
	for (auto msg : m_Output) {
		msg->render(dt);
	}
}

void Console::Activate(bool p_isActive)
{
	m_isActive = p_isActive;
}

bool Console::IsActive() const
{
	return m_isActive;
}