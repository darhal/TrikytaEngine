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
	if (m_Size->x == 0 && m_Size->y == 0)
		setSize(Vec2i(SpriteFramesCoords->at(m_CurrentAnimFrameID).w, SpriteFramesCoords->at(m_CurrentAnimFrameID).h));
	SetRotationCenter(Vec2i(m_Size->x / 2, m_Size->y / 2));
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
	LastDeltaTime = LastDeltaTime + dt;
	if (LastDeltaTime > RequiredTimeToUpdate) {
		if (m_CurrentAnimFrameID > m_MaxFrames) {
			m_CurrentAnimFrameID = 0;
		}
		m_SourceDrawCoord = SpriteFramesCoords->at(m_CurrentAnimFrameID); // override the Sprite set functions!
		LastDeltaTime = 0;
		m_CurrentAnimFrameID++;
	}
	Drawable::render(dt);
	//SDL_RenderCopyEx(TrikytaEngine::getEngine()->getRenderer(), m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
}

void Animation::loadSpirteFrames()
{

}