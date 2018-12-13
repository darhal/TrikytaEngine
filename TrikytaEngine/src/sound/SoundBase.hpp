#pragma once
#include "SoundManager.hpp"
#include <core/Common/defines.h>
#include "core/Common/Macros.hpp"
#include <vector>
#include <functional>

enum SoundType{
	EFFECT,
	MUSIC,
	NONE,
};

enum MusicStatus
{
	PLAYING,
	STOPPED,
	PAUSED,
};


class SoundBase
{
	REGISTER_EVENT(ON_SOUND_FINISH, void());
public:
	inline int getChannel() { return m_Channel; }
	inline const SoundType& getSoundType() { return m_Type; }
	bool IsPaused();
	bool IsPlaying();
	bool IsStopped();
	const MusicStatus& getStatus();
protected:
	SoundBase(const SoundType& type=NONE) {
		SoundManager::GetSoundManager()->add(this);
		m_Channel = -128;
		m_Type = type;
		m_status = STOPPED;
	}

	virtual ~SoundBase() {
		SoundManager::GetSoundManager()->remove(this);
	}

	int m_Channel;
	SoundType m_Type;
	MusicStatus m_status;
private:
	friend class SoundManager;
};

inline bool SoundBase::IsPaused()
{
	return (m_status == PAUSED);
}


inline bool SoundBase::IsStopped()
{
	return (m_status == STOPPED);
}

inline bool SoundBase::IsPlaying()
{
	return (m_status == PLAYING);
}

inline const MusicStatus& SoundBase::getStatus()
{
	return m_status;
}