#include "Console.h"
#include "UI/UIText.h"

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

Console::Console()
{
	m_Output.reserve(MAX_CONSOLE_OUPUT);
	m_LastInputY = 300;
}

void Console::outputConsole(std::string p_Text)
{
	m_LastInputY = m_Output.empty() ? m_LastInputY : m_LastInputY-m_Output.back()->getSize().y;
	auto output = UI::Text::createText(p_Text, "assets/fonts/DroidSans.ttf", 14, Vec2i(0, m_LastInputY), Color{ 255,255,255, 255 });
	m_Output.emplace_back(output);
}
