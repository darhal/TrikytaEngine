#pragma once
#include <SDL/SDL.h>
#include <vector>
#include "core/Common/Vec2.h"

namespace Physics2D {
	class PhysicsBody;
}

class Camera
{
public:
	static Camera* CreateCamera();
	void FollowObject();

	void addObjectToCamera(class TiledMap*);
	void setCameraPosition(Vec2i);
	void moveCamera(Vec2i);
	Camera();
	~Camera();
private:
	Vec2i m_CamPos;
	Vec2i m_CamSize;
	std::vector<class TiledMap*> m_CameraAttachedObjects;
	//Physics2D::PhysicsBody* m_CameraBody;
};

