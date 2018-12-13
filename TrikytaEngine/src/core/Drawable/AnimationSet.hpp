#pragma once
#include <string>
#include <core/Common/defines.h>
#include "Sprite.h"

class AnimationSet : public Sprite
{
public:
	typedef Sprite Super;

protected:
	AnimationSet(const std::string& p_Filename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, int p_FramesMax, int p_FirstFrame) :
		Sprite(p_Filename, p_Size, p_Position), m_MaxFrames(p_FramesMax), m_CurrentAnimFrameID(p_FirstFrame)
	{
		init();
	}
	AnimationSet(const std::string& p_SpritesFilename, const std::string& p_ParserFilename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, float p_RequiredTimeToUpdate) :
		Sprite(p_SpritesFilename, p_Size, p_Position), AnimFilename(p_ParserFilename), RequiredTimeToUpdate(p_RequiredTimeToUpdate)
	{
		init();
	}
public:
	AnimationSet() {} // default ctor
	virtual ~AnimationSet();
	virtual bool init() override;
	virtual void render(float) override;
	void loadAnimationCoordinates();
	//bool initWithAnimationPack();
	void setAnimation(const std::string&);

	//virtual Vec2<int> getSize() { return Vec2i(m_CurrentFrame->at(0).w, m_CurrentFrame->at(0).h); }

	static AnimationSet* Create(const std::string& p_Filename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, int p_FramesMax, int p_FirstFrame = 0)
	{
		return new AnimationSet(p_Filename, p_Size, p_Position, p_FramesMax, p_FirstFrame);
	}

	static AnimationSet* Create(const std::string& p_SpritesFilename, const std::string& p_ParserFilename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, float p_RequiredTimeToUpdate = 1 / 60)
	{
		return new AnimationSet(p_SpritesFilename, p_ParserFilename, p_Size, p_Position, p_RequiredTimeToUpdate);
	}
private:
	int m_CurrentAnimFrameID;
	int m_MaxFrames;
	float LastDeltaTime;
	float RequiredTimeToUpdate;
	std::string AnimFilename;
	std::string m_CurrentAnimation;
	std::vector<SDL_Rect>* m_CurrentFrame;
	std::map<std::string, std::vector<SDL_Rect>*> SpriteFramesCoords;
};