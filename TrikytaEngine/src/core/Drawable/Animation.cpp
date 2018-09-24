#include "Animation.h"
#include <SDL/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sstream>
#include <iostream>
#include <vector>

Animation::~Animation()
{
	/*for (auto itr : *SpriteFramesCoords) {
		FREE(itr);
	}*/
	FREE(SpriteFramesCoords);
};

bool Animation::init()
{
	Super::init();
	SpriteFramesCoords = new std::vector<SDL_Rect>;
	loadAnimationCoordinates();
	m_DestinationDrawCoord.w = SpriteFramesCoords->at(m_CurrentAnimFrameID).w;
	m_DestinationDrawCoord.h = SpriteFramesCoords->at(m_CurrentAnimFrameID).h;
	return true;
}

/*bool Animation::initWithAnimationPack()
{
	Super::init();
	loadAnimationCoordinates();
	return true;
}*/

void Animation::loadAnimationCoordinates()
{
	using namespace std;
	ifstream AnimFile;
	AnimFile.open(AnimFilename);
	string line;
	if (AnimFile.is_open())
	{
		Log("[INFO] Reading file %s ...", AnimFilename.c_str());
		while (getline(AnimFile, line))
		{
			string coord;
			int FrameCoord[4] = {};
			int i = 0;
			bool isFirstLine = true;
			istringstream LineStream(line);
			while (getline(LineStream, coord, ',')) {

				if (!isFirstLine) {
					FrameCoord[i] = stoi(coord);
					i++;
				}
				if (isFirstLine) { isFirstLine = false; };
			}
			//Log("Values pushed : {%d, %d, %d, %d}", FrameCoord[0], FrameCoord[1], FrameCoord[2], FrameCoord[3]);
			SpriteFramesCoords->push_back({ FrameCoord[0], FrameCoord[1], FrameCoord[2], FrameCoord[3] });
		}
		AnimFile.close();
	}else {
		Log("[ERROR] Attempt reading file %s Failed", AnimFilename.c_str());
	}
	m_MaxFrames = SpriteFramesCoords->size() - 1;
}

void Animation::render(float dt)
{
	Object::render(dt);
	LastDeltaTime = LastDeltaTime + dt;
	if (LastDeltaTime > RequiredTimeToUpdate) {
		m_CurrentAnimFrameID++;
		LastDeltaTime = 0;
	}

	if (m_CurrentAnimFrameID > m_MaxFrames) {
		m_CurrentAnimFrameID = 0;
	}
	m_SourceDrawCoord = SpriteFramesCoords->at(m_CurrentAnimFrameID); // override the Sprite set functions!
	/*m_DestinationDrawCoord.w = SpriteFramesCoords->at(m_CurrentAnimFrameID).w;
	m_DestinationDrawCoord.h = SpriteFramesCoords->at(m_CurrentAnimFrameID).h;*/
	m_DestinationDrawCoord = SDL_Rect{ m_Position.x, m_Position.y,  SpriteFramesCoords->at(m_CurrentAnimFrameID).w, SpriteFramesCoords->at(m_CurrentAnimFrameID).h };
	SDL_RenderCopyEx(TrikytaEngine::getEngine()->getRenderer(), m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
}

void Animation::loadSpirteFrames()
{

}