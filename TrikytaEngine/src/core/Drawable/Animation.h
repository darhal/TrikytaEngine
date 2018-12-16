#pragma once
#include <core/Common/defines.h>
#include "Sprite.h"

class Animation : public Sprite
{
public:
	typedef Sprite Super;

protected:
	Animation(const std::string& p_Filename, Vec2<int> p_Size, Vec2<int> p_Position, int p_FramesMax, int p_FirstFrame, bool r) :
		Sprite(p_Filename, p_Size, p_Position, r), m_MaxFrames(p_FramesMax), m_CurrentAnimFrameID(p_FirstFrame)
	{
		init();
	}
	Animation(const std::string& p_SpritesFilename, const std::string& p_ParserFilename, Vec2<int> p_Size, Vec2<int> p_Position, float p_RequiredTimeToUpdate, bool r) :
		Sprite(p_SpritesFilename, p_Size, p_Position, r), AnimFilename(p_ParserFilename), RequiredTimeToUpdate(p_RequiredTimeToUpdate)
	{
		init();
	}
public:
	virtual ~Animation();
	virtual bool init() override;
	virtual void render(float) override;
	void loadSpirteFrames(); //TODO LOAD SPRITE BY SPRITE!
	void loadAnimationCoordinates();
	//bool initWithAnimationPack();

	virtual Vec2<int> getSize() { return Vec2i(SpriteFramesCoords->at(0).w, SpriteFramesCoords->at(0).h); }
	inline std::vector<SDL_Rect>* getSpriteFramesCoords() { return SpriteFramesCoords; }

	static Animation* Create(const std::string& p_Filename, Vec2<int> p_Size, Vec2<int> p_Position, int p_FramesMax, int p_FirstFrame = 0, bool r=true)
	{
		return new Animation(p_Filename, p_Size, p_Position, p_FramesMax, p_FirstFrame, r);
	}

	static Animation* Create(const std::string& p_SpritesFilename, const std::string& p_ParserFilename, Vec2<int> p_Size, Vec2<int> p_Position, float p_RequiredTimeToUpdate = 1 / 60, bool r = true)
	{
		return new Animation(p_SpritesFilename, p_ParserFilename, p_Size, p_Position, p_RequiredTimeToUpdate, r);
	}
private:
	int m_CurrentAnimFrameID;
	int m_MaxFrames;
	float LastDeltaTime;
	float RequiredTimeToUpdate;
	std::string AnimFilename;
	std::vector<SDL_Rect>* SpriteFramesCoords;
};