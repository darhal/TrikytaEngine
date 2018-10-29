#include "Camera.h"
#include "core/Common/TrikytaEngine.h"
#include "core/TiledMap/TiledMap.h"
#include "core/Physics/PhysicsEngine.h"
//http://lazyfoo.net/tutorials/SDL/30_scrolling/index.php

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
	p_drawable->setAffectedByCamera(true);
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
	//m_CameraBody->SetTransform(Vec2f((float)m_CamPos.x, (float)m_CamPos.y), 0.f);
}

/*	m_CameraBody = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), BodyType::STATIC,
		BodyShape::BOX, BodyParams{ 1, 1, 0.f, true },
		Vec2f{ (float)m_CamPos.x, (float)m_CamPos.y},
		std::vector<Vec2f>{Vec2f((float)m_CamSize.x-50, (float)m_CamSize.y-50)}
	);
*/