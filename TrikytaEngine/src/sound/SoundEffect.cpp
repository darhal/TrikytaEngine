#include "SoundEffect.hpp"
#include <core/Common/defines.h>

SoundEffect::SoundEffect(const std::string& filename) : SoundBase(SoundType::EFFECT)
{
	m_Chunk = Mix_LoadWAV(filename.c_str());
	if (m_Chunk == nullptr) {
		LogConsole(LogError, "Failed to load sound effect with filename : %s,  %s", filename.c_str(), Mix_GetError());
	}
}

void SoundEffect::Play(int loop, int channel)
{
	m_Channel = Mix_PlayChannel(channel, m_Chunk, loop);
	if (m_Channel == -1){
		LogConsole(LogError, "Unable to play effect %s", Mix_GetError());
	}
}

void SoundEffect::setVolume(int vol)
{
	if (Mix_Playing(m_Channel) == false) return;
	int volume = (vol < 0) ? 0 : (vol > 100) ? 100 : vol; // p is clamped between 0 and 100
	Mix_Volume(m_Channel, int((float(volume) / 100.f)*MIX_MAX_VOLUME));
}

int SoundEffect::getVolume()
{
	if (Mix_Playing(m_Channel) == false) return 0;
	return int((Mix_Volume(m_Channel, -1) / float(MIX_MAX_VOLUME))*100);
}

SoundEffect::~SoundEffect()
{
	if (m_Chunk != nullptr)
		Mix_FreeChunk(m_Chunk);
}