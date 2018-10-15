#pragma once
#include "UI/UIBase.h"
#include "ConsoleText.h"

class ConsoleCommandField : public UI::Base, public ConsoleText
{
public:
	typedef std::function<void(std::vector<std::string>)> CmdFunc;
	static ConsoleCommandField* createConsoleCommandField(std::string p_Text, Vec2i p_Pos, Color p_Color)
	{
		return new ConsoleCommandField(p_Text, p_Pos, p_Color);
	}

	virtual void OnUIClick(Vec2i, bool) override;
	virtual void OnUIFocus(bool) override;
	inline virtual Vec2i getPos() override;
	inline virtual Vec2i getSize() override;

	void render(float);
	void ProcessEventHelper(SDL_Event&);
	void PorcessEvents(union SDL_Event&) override;
	void CommandExec(std::string& cmd);
	~ConsoleCommandField();
protected:
	ConsoleCommandField(std::string, Vec2i, Color);
private:
	std::vector<std::string> m_Commands;
	std::vector<CmdFunc> m_CmdFunctions;
};