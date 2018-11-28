#include "ConsoleText.h"
#include "core/Common/TrikytaEngine.h"
#include "misc/Font.h"

ConsoleText::ConsoleText(const std::string& p_Text, const Vec2i& p_Pos, const Color& p_Color):
	m_Color(p_Color)
{
	init(p_Text, p_Pos, p_Color);
}

ConsoleText::~ConsoleText()
{
	SDL_DestroyTexture(m_Texture);
}

bool ConsoleText::init(const std::string& p_Text, const Vec2i& p_Pos, const Color& p_Color)
{
	auto font = Console::getConsoleFont();
	//SDL_Surface* textSurface = TTF_RenderText_Shaded(font->getFont(), p_Text.c_str(), { p_Color.r,p_Color.g, p_Color.b }, {0,0,0,255});
	SDL_Surface* textSurface = TTF_RenderText_Blended(font->getFont(), p_Text.c_str(), { p_Color.r,p_Color.g, p_Color.b, p_Color.a });
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	int temp_w, temp_h;
	TTF_SizeText(font->getFont(), p_Text.c_str(), &temp_w, &temp_h);
	m_DestinationDrawCoord = SDL_Rect{ p_Pos.x, p_Pos.y ,temp_w, temp_h };
	SDL_FreeSurface(textSurface);
	m_Text = p_Text;
	return true;
}

void ConsoleText::renderConsoleText()
{
	SDL_RenderCopy(TrikytaEngine::getEngine()->getRenderer(), m_Texture, NULL, &m_DestinationDrawCoord);
}

void ConsoleText::updateText(const std::string& p_Text)
{
	m_Text = p_Text;
	this->updateTextHelper();
}

void ConsoleText::updateTextHelper()
{
	auto font = Console::getConsoleFont();
	auto p_Pos = getPosition();
	SDL_Surface* textSurface = TTF_RenderText_Blended(font->getFont(), m_Text.c_str(), { m_Color.r,m_Color.g, m_Color.b, m_Color.a });
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	int temp_w, temp_h;
	TTF_SizeText(font->getFont(), m_Text.c_str(), &temp_w, &temp_h);
	m_DestinationDrawCoord = SDL_Rect{ p_Pos.x, p_Pos.y ,temp_w, temp_h };
	SDL_FreeSurface(textSurface);
}

void ConsoleText::setColor(Color p_Color)
{
	m_Color = p_Color;
	auto font = Console::getConsoleFont();
	SDL_Surface* textSurface = TTF_RenderText_Solid(font->getFont(), m_Text.c_str(), { m_Color.r,m_Color.g, m_Color.b, m_Color.a });
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	SDL_FreeSurface(textSurface);
}