#include "UIText.h"
#include "core/Common/TrikytaEngine.h"

using namespace UI;

Text::Text(std::string p_Text, std::string p_Font, uint8 p_TextSize,Vec2i p_Pos, Color p_Color):
	Drawable(p_Pos, Vec2i(0, 0)),
	m_Text(p_Text),
	m_FontPath(p_Font),
	m_TextSize(p_TextSize),
	m_Color(p_Color)
{
	init();
}

bool Text::init()
{
	m_Font = TTF_OpenFont(m_FontPath.c_str(), m_TextSize);
	if (m_Font == NULL) {
		Log("ERROR: loading font path %s", m_FontPath.c_str());
	}
	SDL_Color color = {m_Color.r,m_Color.g, m_Color.b};
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font, m_Text.c_str(), color);
	if (textSurface == NULL) {
		Log("ERROR: loading text with font %s", m_FontPath.c_str());
		return false;
	}
	m_TextTexture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);

	int strLen = m_Text.length();
	setSize(Vec2i((int)(strLen*m_TextSize), (int)(m_TextSize)));

	SDL_FreeSurface(textSurface);
	//TTF_CloseFont(font);
	return true;
}

void Text::render(float dt)
{
	SDL_RenderCopy(ENGINE->getRenderer(), m_TextTexture, NULL, &m_DestinationDrawCoord);
}

void Text::UpdateText(std::string p_Text)
{
	//auto font = TTF_OpenFont(m_FontPath.c_str(), m_TextSize);
	
	if (m_Font == NULL) {
		Log("ERROR: loading font path %s", m_FontPath.c_str());
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_Font, p_Text.c_str(), SDL_Color{ m_Color.r,m_Color.g, m_Color.b });
	if (textSurface == NULL) {
		Log("ERROR: loading text with font %s", m_FontPath.c_str());
		return;
	}
	SDL_DestroyTexture(m_TextTexture);
	m_TextTexture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), textSurface);
	int strLen = p_Text.length();
	setSize(Vec2i((int)(strLen*m_TextSize), (int)(m_TextSize)));
	m_Text = p_Text;
	SDL_FreeSurface(textSurface);
}