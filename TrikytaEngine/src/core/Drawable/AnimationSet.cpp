#include "AnimationSet.hpp"
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iostream>
#include <vector>

AnimationSet::~AnimationSet()
{
	for (auto& itr : SpriteFramesCoords) {
		//FREE(itr.second);
	}
};

bool AnimationSet::init()
{
	Super::init();
	LastDeltaTime = 0.f;
	loadAnimationCoordinates();
	if (m_Size.x == 0 && m_Size.y == 0)
		setSize(Vec2i(m_CurrentFrame->at(m_CurrentAnimFrameID).w, m_CurrentFrame->at(m_CurrentAnimFrameID).h));
	SetRotationCenter(Vec2i(m_Size.x / 2, m_Size.y / 2));
	return true;
}

void AnimationSet::loadAnimationCoordinates()
{
	using namespace std;
	ifstream AnimFile;
	AnimFile.open(AnimFilename);
	string line;
	if (AnimFile.is_open()){
		LogConsole(MESSAGE_TYPE::INFO, "Reading file %s ...", AnimFilename.c_str());
		string action = "";
		while (getline(AnimFile, line)){
			if (line.at(0) == '#') {
				action = line;
				action.erase(action.begin());
				if (SpriteFramesCoords.count(action) == 0) {
					SpriteFramesCoords[action] = new std::vector<SDL_Rect>;
				}
				continue;
			}
			string readBuffer;
			int FrameCoord[4] = {};
			int i = 0;
			istringstream LineStream(line);
			while (getline(LineStream, readBuffer, ',')) {
				if (static_cast<char>(readBuffer.at(0)) >= 48 && static_cast<char>(readBuffer.at(0)) <= 57) { // number according to ascii table
					FrameCoord[i] = stoi(readBuffer);
					i++;
				}
			}
			if (i > 0) {
				SpriteFramesCoords[action]->push_back({ FrameCoord[0], FrameCoord[1], FrameCoord[2], FrameCoord[3] });
			}
		}
		setAnimation(action);
		AnimFile.close();
	}else {
		LogConsole(MESSAGE_TYPE::ERROR, "Attempt reading file %s Failed", AnimFilename.c_str());
		//TODO: Should delete the object!
	}
}

void AnimationSet::render(float dt)
{
	LastDeltaTime = LastDeltaTime + dt;
	if (LastDeltaTime > RequiredTimeToUpdate) {
		if (m_CurrentAnimFrameID > m_MaxFrames) {
			m_CurrentAnimFrameID = 0;
		}
		m_SourceDrawCoord = m_CurrentFrame->at(m_CurrentAnimFrameID); // override the Sprite set functions!
		LastDeltaTime = 0;
		m_CurrentAnimFrameID++;
	}
	Drawable::render(dt);
}

void AnimationSet::setAnimation(const std::string& p_name)
{
	if (SpriteFramesCoords.count(p_name) != 0 && m_CurrentAnimation != p_name) {
		m_CurrentAnimFrameID = 0;
		LastDeltaTime = 0.f;
		m_CurrentFrame = SpriteFramesCoords[p_name];
		m_MaxFrames = m_CurrentFrame->size() - 1;
		m_CurrentAnimation = p_name;
	}
}
