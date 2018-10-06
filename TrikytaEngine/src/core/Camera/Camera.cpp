#include "Camera.h"
#include "core/Common/TrikytaEngine.h"

Camera* Camera::CreateCamera()
{
	return new Camera();
}

//http://lazyfoo.net/tutorials/SDL/30_scrolling/index.php

Camera::Camera():m_CamPos(0,0), m_CamSize(0,0)
{
}


void Camera::FollowObject()
{

}

Camera::~Camera()
{
}
