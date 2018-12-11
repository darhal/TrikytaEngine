#include "UIImage.hpp"
#include <core/Common/TrikytaEngine.h>
#include "UIManager.h"
#include <SDL/SDL_image.h>

using namespace UI;

Image::Image(const std::string& imagePath, const Vec2i& p_Pos, const Vec2i& p_Size) : m_Pos(p_Pos), m_Size(p_Size)
{
	Manager::addElement(this, true);
	buildWidget(imagePath);
	m_WidgetBounderies = SDL_Rect{ m_Pos.x, m_Pos.y, m_Size.x, m_Size.y };
}

void Image::buildWidget(const std::string& imagePath)
{
	SDL_Surface* TempSurface = IMG_Load(imagePath.c_str());
	if (TempSurface == NULL) {
		Log("Unable to load image : %s : %s", imagePath.c_str(), IMG_GetError());
		return;
	}
	// Convert SDL surface to a texture
	if ((m_Texture = SDL_CreateTextureFromSurface(TrikytaEngine::getEngine()->getRenderer(), TempSurface)) == NULL) {
		Log("Unable to create SDL Texture : %s : %s", imagePath.c_str(), IMG_GetError());
		return;
	}
	// Grab dimensions too m_NormalSize!
	Vec2i m_NormalSize;
	SDL_QueryTexture(m_Texture, NULL, NULL, &m_NormalSize.x, &m_NormalSize.y);
	SDL_FreeSurface(TempSurface);

	if (m_Size == Vec2i(0, 0))
		m_Size = m_NormalSize;
}

void Image::render(float)
{
	auto r = ENGINE->getRenderer();
	SDL_RenderCopy(r, m_Texture, NULL, &m_WidgetBounderies);
}

void Image::ChangeTexture(const std::string& imagePath)
{
	SDL_Surface* TempSurface = IMG_Load(imagePath.c_str());
	if (TempSurface == NULL) {
		Log("Unable to load image : %s : %s", imagePath.c_str(), IMG_GetError());
		return;
	}
	SDL_DestroyTexture(m_Texture);
	m_Texture = NULL;
	if ((m_Texture = SDL_CreateTextureFromSurface(TrikytaEngine::getEngine()->getRenderer(), TempSurface)) == NULL) {
		Log("Unable to create SDL Texture : %s : %s", imagePath.c_str(), IMG_GetError());
		return;
	}
	SDL_FreeSurface(TempSurface);
}