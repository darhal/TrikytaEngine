#pragma once
#include <string>
#include "SoundBase.hpp"
#include <SDL/SDL_mixer.h>

enum PlayStopType
{
	NORMAL,
	FADE,
};

enum Fading
{
	OUT,
	IN,
	NO_FADING,
};

class Music : public SoundBase
{
public:
	Music(const std::string&);
	~Music();
	void Play(int, PlayStopType = PlayStopType::NORMAL, int=0, int=0);
	void Pause();
	void Stop(PlayStopType = PlayStopType::NORMAL, int=0);
	void Resume();
	void setVolume(int = 100);
	int getVolume();
	void setPosition(double);
	void Rewind();
	Fading getFadingStatus();
private:
	friend class SoundManager;
	Mix_Music* m_music;
};


