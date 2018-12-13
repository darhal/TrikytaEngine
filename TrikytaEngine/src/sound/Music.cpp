#include "Music.hpp"
#include <core/Common/defines.h>
#include <core/Utility/TimerManager.h>

Music::Music(const std::string& filename) : m_music(nullptr), SoundBase(SoundType::MUSIC)
{
	m_music = Mix_LoadMUS(filename.c_str());
	if (m_music == nullptr) {
		LogConsole(LogError, "Failed to load sound with filename : %s, ", filename.c_str(), Mix_GetError());
	}
}

void Music::Play(int playTimes, PlayStopType playtype, int fadetime, int pos)
{
	int res = 0;
	if (Mix_PlayingMusic() == 0){
		if (playtype == NORMAL) {
			res = Mix_PlayMusic(m_music, playTimes);
		}else if (playtype == FADE) {
			Mix_FadeInMusicPos(m_music, playTimes, fadetime, pos);
		}
	}
	if (res == -1) {
		LogConsole(LogError, "Unable to play a music %s", Mix_GetError());
	}else{
		m_status = PLAYING;
	}
}

void Music::Pause()
{
	if (!(Mix_PausedMusic() == 1)) {//If the music is playing
		Mix_PauseMusic(); //Pause the music
		m_status = PAUSED;
	}
}

void Music::Stop(PlayStopType pausetype, int fadetime)
{
	int res = 0;
	if (pausetype == NORMAL) {
		res = Mix_HaltMusic();
	}else if (pausetype == FADE) {
		res = Mix_FadeOutMusic(fadetime);
		TimerManager::CreateTimer([=]() {m_status = STOPPED; }, fadetime, 1, true);
	}
	if (res == -1) {
		LogConsole(LogError, "Unable to stop a music %s", Mix_GetError());
	}else if(pausetype != FADE) {
		m_status = STOPPED;
	}
}

void Music::Resume()
{
	if (Mix_PausedMusic() == 1) {//If the music is paused
		Mix_ResumeMusic();	//Resume the music
		m_status = PLAYING;
	}
}

void Music::setVolume(int vol)
{
	if (m_status == STOPPED) { return; }
	int volume = (vol < 0) ? 0 : (vol > 100) ? 100 : vol; // p is clamped between 0 and 100
	Mix_VolumeMusic(int((float(volume) / 100.f)*MIX_MAX_VOLUME));
}

int Music::getVolume()
{
	if (m_status == STOPPED) { return 0; }
	return int((Mix_VolumeMusic(-1) / float(MIX_MAX_VOLUME)) * 100);
}

void Music::setPosition(double pos)
{
	if (m_status == STOPPED) { return; }
	Mix_RewindMusic();
	if (Mix_SetMusicPosition(pos) == -1) {
		LogConsole(LogError, "Music set position : %s\n", Mix_GetError());
	}
}

Fading Music::getFadingStatus()
{
	if (m_status == STOPPED) { return NO_FADING; }
	Fading fading;
	switch (Mix_FadingMusic()) {
	case MIX_NO_FADING:
		fading = NO_FADING;
		break;
	case MIX_FADING_OUT:
		fading = OUT;
		break;
	case MIX_FADING_IN:
		fading = IN;
		break;
	}
	return fading;
}

void Music::Rewind()
{
	if (m_status == STOPPED) { return; }
	Mix_RewindMusic();
}

Music::~Music()
{
	if (m_status != STOPPED) {//If the music is playing or paused
		Mix_HaltMusic(); //stop the music
	}
	if (m_music != nullptr)
		Mix_FreeMusic(m_music);
}
