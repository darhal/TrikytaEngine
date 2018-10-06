#include "ConsoleText.h"
#include "core/Common/TrikytaEngine.h"
#include "misc/Font.h"

ConsoleText::ConsoleText(std::string p_Text,Vec2i p_Pos, Color p_Color)
{
	init(p_Text, p_Pos, p_Color);
}

ConsoleText::~ConsoleText()
{
	SDL_DestroyTexture(m_Texture);
}

bool ConsoleText::init(std::string p_Text, Vec2i p_Pos, Color p_Color)
{
	auto font = Console::getConsoleFont();
	SDL_Surface* textSurface = TTF_RenderText_Shaded(font->getFont(), p_Text.c_str(), { p_Color.r,p_Color.g, p_Color.b }, {0,0,0,255});
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	int temp_w, temp_h;
	TTF_SizeText(font->getFont(), p_Text.c_str(), &temp_w, &temp_h);
	m_DestinationDrawCoord = SDL_Rect{ p_Pos.x, p_Pos.y ,temp_w, temp_h };
	SDL_FreeSurface(textSurface);
	return true;
}

void ConsoleText::renderConsoleText()
{
	SDL_RenderCopy(TrikytaEngine::getEngine()->getRenderer(), m_Texture, NULL, &m_DestinationDrawCoord);
}