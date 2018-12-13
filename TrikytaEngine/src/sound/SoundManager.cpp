#include "SoundManager.hpp"
#include "core/Common/Macros.hpp"
#include <core/Common/defines.h>
#include "SoundBase.hpp"
#include <SDL/SDL_mixer.h>
#include <algorithm>
#include "Music.hpp"

SoundManager* SoundManager::__SoundManager__ = nullptr;

SoundManager* SoundManager::init()
{
	if (__SoundManager__ == nullptr) {
		__SoundManager__ = new SoundManager();
		Mix_ChannelFinished(OnEffectFinish);
		Mix_HookMusicFinished(OnMusicFinish);
		__SoundManager__->m_SoundObjects.reserve(6);
	}
	return __SoundManager__;
}

SoundManager* SoundManager::GetSoundManager()
{
	return __SoundManager__;
}

void SoundManager::add(SoundBase* s)
{
	m_SoundObjects.emplace_back(s);
}

void SoundManager::remove(SoundBase* s)
{
	std::vector<SoundBase*>::iterator position = std::find(m_SoundObjects.begin(), m_SoundObjects.end(), s);
	if (position != m_SoundObjects.end()) {
		m_SoundObjects.erase(position);
	}
}

void SoundManager::AllocateChannels(int channels)
{
	Mix_AllocateChannels(channels);
}


//Callbacks!
void SoundManager::OnEffectFinish(int channel)
{
	for (auto itr : GetSoundManager()->m_SoundObjects) {
		if (itr->getSoundType() == EFFECT && itr->getChannel() == channel) {
			TRIGGER_EVENT_WITH_TARGET(ON_SOUND_FINISH, itr);
			itr->m_Channel = -999; // Invalid channel!
		}
	}
}

void SoundManager::OnMusicFinish()
{
	for (auto itr : GetSoundManager()->m_SoundObjects) {
		if (itr->getSoundType() == MUSIC && itr->getStatus() == PLAYING) {
			auto music = dynamic_cast<Music*>(itr);
			TRIGGER_EVENT_WITH_TARGET(ON_SOUND_FINISH, music);
			itr->m_Channel = -999; // Invalid channel!
			music->m_status = STOPPED;
		}
	}
}