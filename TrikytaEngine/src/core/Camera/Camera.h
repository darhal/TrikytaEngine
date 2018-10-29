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

	void addObjectToCamera(class Drawable*);
	void setCameraPosition(Vec2i);
	void moveCamera(Vec2i);
	Camera();
	~Camera();

	inline Vec2i getCameraPosition() { return m_CamPos; }
	inline Vec2i getCameraSize() { return m_CamSize; }
private:
	Vec2i m_CamPos;
	Vec2i m_CamSize;
	std::vector<class Drawable*> m_CameraAttachedObjects;
	//SDL_Rect m_rect;
};


/*#include "core/Common/Vec2.h"
#include "core/Drawable/Drawable.h"

class Camera
{
public:

	Vec2i m_offset;    //world offset

	unsigned camera_width;
	unsigned camera_height;

	Drawable* m_follow;

	Camera() : m_offset(0, 0) { camera_width = ENGINE->GetScreenWidth(); camera_height = ENGINE->GetScreenWidth(); };
	Camera(Vec2i& p_offset) { m_offset = p_offset;  };

	void TransformPosition(Vec2i &pos)
	{
		pos += m_offset;
	};

	void RecalculateFollowing()
	{
		auto sprite_pos = m_follow->getPosition();
		auto sprite_w = m_follow->getSize().x;
		auto sprite_h = m_follow->getSize().y;
		auto sprite_center_offset = Vec2i((sprite_w / 2), (sprite_h / 2));

		auto sprite_center = sprite_pos + sprite_center_offset;

		m_offset = Vec2i((sprite_center.x - (camera_width / 2)), (sprite_center.y - (camera_height / 2)));
	};

};*/