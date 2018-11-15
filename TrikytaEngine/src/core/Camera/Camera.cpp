#include "Camera.h"
#include "core/Common/TrikytaEngine.h"
#include "core/TiledMap/TiledMap.h"
#include "core/Physics/PhysicsEngine.h"

Camera* Camera::CreateCamera()
{
	return new Camera();
}


Camera::Camera():m_CamPos(0,0), m_CamSize(ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight())
{
	//m_rect = SDL_Rect{ 0,0,(int)ENGINE->GetScreenWidth(),(int)ENGINE->GetScreenHeight() };
}

void Camera::FollowObject()
{

}

Camera::~Camera()
{
}

void Camera::addObjectToCamera(Drawable* p_drawable)
{
	m_CameraAttachedObjects.emplace_back(p_drawable);
	p_drawable->setAffectedByCamera(this);
}

void Camera::moveCamera(Vec2i p_Offset)
{
	m_CamPos += p_Offset;
	for (auto& obj : m_CameraAttachedObjects)
	{
		p_Offset.x = -p_Offset.x;
		//obj->translateMap(p_Offset);
	}
}

void Camera::setCameraPosition(Vec2i p_Pos)
{
	m_CamPos = p_Pos;
	for (auto& obj : m_CameraAttachedObjects)
	{
		obj->updateRenderPositionFromCamera(m_CamPos);
	}
}