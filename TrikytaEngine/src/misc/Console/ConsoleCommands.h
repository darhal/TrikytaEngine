#pragma once
#include <vector>
#include "ConsoleCommandField.h"



class ConsoleCommands
{
public:
	ConsoleCommands() {}

	void initCommands();
private:
	std::vector<CommandData> m_ArgFunc;
};