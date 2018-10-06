#pragma once
#include <SDL/SDL.h>
#include "core/Common/Vec2.h"

class Camera
{
public:
	static Camera* CreateCamera();
	void FollowObject();

	Camera();
	~Camera();
private:
	Vec2i m_CamPos;
	Vec2i m_CamSize;
};

