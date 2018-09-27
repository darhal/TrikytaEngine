#pragma once
#include <string>
#include <vector>

#define CONSOLE_TEXT_SIZE	14
#define START_POS_X			25
#define FONT_PATH			"Engine_Assets/fonts/DroidSans.ttf"

enum class MESSAGE_TYPE {
	ERROR,
	WARNING,
	INFO,
};

class Console
{
public:
	static void InitConsole();
	static Console* getConsole();
	static class Font* getConsoleFont() {
		return m_Font;
	};

	void outputConsole(std::string, struct Color, bool = true);
	void outputConsole(std::string, MESSAGE_TYPE=MESSAGE_TYPE::INFO);
	void removeConsoleMessage();
	void Draw(float);

	void Activate(bool);
	bool IsActive() const;
protected:
	Console();
private:
	static Console* _Console;
	static class Font* m_Font;
	std::vector<class ConsoleText*> m_Output;
	int m_StartPos;
	bool m_isActive;
};