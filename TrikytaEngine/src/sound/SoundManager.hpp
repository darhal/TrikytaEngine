#pragma once
#include <vector>

class SoundManager
{
public:
	static SoundManager* init();
	static SoundManager* GetSoundManager();
	static void OnEffectFinish(int);
	static void OnMusicFinish();

	void add(class SoundBase*);
	void remove(class SoundBase*);
	void AllocateChannels(int);
protected:

private:
	static SoundManager* __SoundManager__;
	std::vector<class SoundBase*> m_SoundObjects;
};