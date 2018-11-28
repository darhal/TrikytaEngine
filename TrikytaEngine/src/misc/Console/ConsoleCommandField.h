#pragma once
#include "UI/Base/UIEditBoxBase.h"
#include "core/Common/Vec2.h"
#include "ConsoleText.h"
#include <map>
#include <deque>

typedef std::function<void(const std::vector<std::string>&)> CmdFunc;

struct CommandData
{
	std::string m_Command;
	CmdFunc m_CmdFunction;
};

class ConsoleCommandField : public UI::EditBoxBase, public ConsoleText
{
public:
	static ConsoleCommandField* createConsoleCommandField(std::string p_Text, Vec2i p_Pos, Color p_Color)
	{
		return new ConsoleCommandField(p_Text, p_Pos, p_Color);
	}

	virtual void OnUIClick(Vec2i, bool) override;
	virtual void OnUIFocus(bool, Vec2i) override;
	inline virtual Vec2i getTextPos() override;
	inline virtual Vec2i getTextSize() override;

	inline virtual Vec2i getSize() override;
	inline virtual Vec2i getPos() override;

	virtual void render(float) override;
	void ProcessEventHelper(SDL_Event&);
	void PorcessEvents(union SDL_Event&) override;
	void CommandExec(std::string& cmd);
	void AddCommandHandler(const std::string&, CmdFunc);
	~ConsoleCommandField();
protected:
	ConsoleCommandField(std::string, Vec2i, Color);
private:
	std::map<std::string, CommandData> m_CmdData;
	std::deque<std::string> m_CmdHistroy;
	int m_CmdHistorySelector;
};