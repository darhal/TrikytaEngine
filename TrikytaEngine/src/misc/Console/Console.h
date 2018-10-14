#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <SDL/SDL.h>

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
	typedef std::function<void(std::vector<std::string>)> CmdFunc;
	static void InitConsole();
	static Console* getConsole();
	static class Font* getConsoleFont() {
		return m_Font;
	};

	void outputConsole(std::string, struct Color, bool = true);
	void outputConsole(std::string, MESSAGE_TYPE=MESSAGE_TYPE::INFO);
	void removeConsoleMessage();
	void Draw(float);
	void initConsoleCommandField();

	void Activate(bool);
	bool IsActive() const;
	int getStartYPos() { return m_StartPos; }
	void ProcessConsole(SDL_Event&);
	void CommandExec(std::string&);
protected:
	Console();
private:
	static Console* _Console;
	static class Font* m_Font;
	SDL_Rect m_ConsoleBoundries;
	std::vector<class ConsoleText*> m_Output;
	ConsoleText* m_CommandField;
	int m_StartPos;
	bool m_isActive;
	std::vector<std::string> m_Commands;
	std::vector<CmdFunc> m_CmdFunctions;
};