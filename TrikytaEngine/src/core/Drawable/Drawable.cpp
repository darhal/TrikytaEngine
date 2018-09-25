#include "Drawable.h"
#include "core/Physics/PhysicsEngine.h"

Drawable::Drawable(Vec2i m_Pos, Vec2i p_Size) :
	m_Position(new Vec2i(m_Pos)), m_Size(new Vec2i(p_Size)),
	m_NormalSize(Vec2i(0, 0)), m_DrawCoord(Vec2i(0, 0)),
	m_DestinationDrawCoord(SDL_Rect{ m_Position->x, m_Position->y, m_Size->x, m_Size->y }),
	m_SourceDrawCoord(SDL_Rect{ m_NormalSize.x, m_NormalSize.y, m_Size->x, m_Size->y }),
	m_Angle(0.f),
	m_RotationCenter(SDL_Point{ m_DestinationDrawCoord.w / 2, m_DestinationDrawCoord.h / 2 }),
	m_Flip(SDL_RendererFlip::SDL_FLIP_NONE)
{}

void Drawable::render(float dt)
{
	if (m_Parent != nullptr) {
		int px = (int)m_Parent->getPosition().x;
		int py = (int)m_Parent->getPosition().y;
		setPosition(Vec2i(px+m_Offset.x, py+m_Offset.y));
	}
}


void Drawable::attachTo(Drawable* obj, Vec2f p_Offset)
{
	Object::attachTo(obj);
	if (obj == nullptr) {
		Log("Error attempt to attach to null object!")
			return;
	}
	followPosition(obj->getVecPosPtr());
	m_Offset = Vec2i((int)(p_Offset.x*m_Size->x), (int)(p_Offset.y*m_Size->y));
}

void Drawable::attachTo(Physics2D::PhysicsBody* p_Phyobj, Vec2f p_Offset)
{
	Object::attachTo(p_Phyobj);
	if (p_Phyobj == nullptr) {
		Log("Error attempt to attach to null object!")
			return;
	}
	m_Offset = Vec2i((int)(p_Offset.x*m_Size->x), (int)(p_Offset.y*m_Size->y));
	//followPosition(obj->getVecPosPtr());
	//FREE();
}