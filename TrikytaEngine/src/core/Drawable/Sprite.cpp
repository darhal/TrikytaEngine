#include <SDL/SDL_image.h>
#include "Sprite.h"
#include <core/Objects/ObjectHandler.h>
#include "core/Common/defines.h"


bool Sprite::init()
{
	SDL_Surface* TempSurface = IMG_Load(m_Filename.c_str());
	if (TempSurface == NULL) {
		Log("Unable to load image : %s : %s", m_Filename.c_str(), IMG_GetError());
		return false;
	}
	// Convert SDL surface to a texture
	if ((m_Texture = SDL_CreateTextureFromSurface(TrikytaEngine::getEngine()->getRenderer(), TempSurface)) == NULL) {
		Log("Unable to create SDL Texture : %s : %s", m_Filename.c_str(), IMG_GetError());
		return false;
	}
	// Grab dimensions too m_NormalSize!
	SDL_QueryTexture(m_Texture, NULL, NULL, &m_NormalSize.x, &m_NormalSize.y);
	SDL_FreeSurface(TempSurface);

	if (m_Size == nullptr)
		*m_Size = m_NormalSize;

	setNormalSize(m_NormalSize);

	return true;
}

void Sprite::render(float dt)
{
	Drawable::render(dt); 
	updatePosition();
	SDL_RenderCopyEx(TrikytaEngine::getEngine()->getRenderer(), m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(m_Texture);
}

