#pragma once
#include <core/Common/Macros.hpp>
#include "core/Common/defines.h"
#include "SoundBase.hpp"
#include <string>
#include <SDL/SDL_mixer.h>

class SoundEffect : public SoundBase
{
public:
	SoundEffect(const std::string&);
	~SoundEffect();
	void Play(int = -1, int = -1);
	void setVolume(int=100);
	int getVolume();
private:
	Mix_Chunk* m_Chunk;
};