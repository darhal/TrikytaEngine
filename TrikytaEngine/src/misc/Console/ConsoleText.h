#pragma once
#include "core/Common/defines.h"
#include "core/Common/Color.h"
#include <string>
#include <SDL/SDL.h>

class ConsoleText
{
public:
	static ConsoleText* createText(std::string p_Text, Vec2i p_Pos, Color p_Color)
	{
		return new ConsoleText(p_Text, p_Pos, p_Color);
	}
	virtual ~ConsoleText();
	void renderConsoleText();
	inline void setSize(const Vec2i& p_Size) { m_DestinationDrawCoord.w = p_Size.x; m_DestinationDrawCoord.h = p_Size.y; }
	inline void setPosition(const Vec2i& p_Position) { m_DestinationDrawCoord.x = p_Position.x; m_DestinationDrawCoord.y = p_Position.y; }
	Vec2i getSize() const { return Vec2i(m_DestinationDrawCoord.w, m_DestinationDrawCoord.h); }
	Vec2i getPosition() { return Vec2i(m_DestinationDrawCoord.x, m_DestinationDrawCoord.y); }

	void updateText(const std::string&);
	void updateTextHelper();
	void setColor(Color);
protected:
	ConsoleText(std::string, Vec2i, Color);
	bool init(std::string, Vec2i, Color);

	std::string m_Text;
private:
	Color m_Color;
	struct SDL_Texture* m_Texture;
	struct SDL_Rect m_DestinationDrawCoord;
	friend class Console;
};
