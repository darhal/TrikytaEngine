#pragma once
#include <string>
#include <vector>
#include "core/Common/defines.h"

class Console
{
public:
	static void InitConsole();
	static Console* getConsole();

	void outputConsole(std::string);
protected:
	Console();
private:
	static Console* _Console;
	std::vector<class UI::Text*> m_Output;
	int m_LastInputY;
};