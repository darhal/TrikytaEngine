#include <SDL/SDL_image.h>
#include "Sprite.h"
#include <core/Objects/ObjectHandler.h>
#include "core/Common/defines.h"


bool Sprite::init()
{
	m_DrawCoord = { 0,0 };

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
	if (&m_Size == nullptr)
		m_Size = m_NormalSize;
	m_SourceDrawCoord = SDL_Rect{ m_DrawCoord.x, m_DrawCoord.y, m_NormalSize.x, m_NormalSize.y };
	m_DestinationDrawCoord = SDL_Rect{ m_Position.x, m_Position.y, m_Size.x, m_Size.y };
	m_Angle = 0.f;
	m_RotationCenter = SDL_Point{ m_DestinationDrawCoord.w / 2, m_DestinationDrawCoord.h / 2 };
	m_Flip = SDL_RendererFlip::SDL_FLIP_NONE;
	return true;
}

void Sprite::render(float dt)
{
	Object::render(dt);
	updateDestinationDrawCoord();
	SDL_RenderCopyEx(TrikytaEngine::getEngine()->getRenderer(), m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(m_Texture);
}

