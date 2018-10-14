#include "Drawable.h"
#include "core/Physics/PhysicsEngine.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Objects/ObjectHandler.h"
/*
TODO:
ISSUES:
-Attach rotations should be optional!
-Add system to handle a bundle of animations, physics (player, entity, charecter...)
*/

Drawable::Drawable(Vec2i m_Pos, Vec2i p_Size, bool p_RegisterInHandler) :
	m_Position(new Vec2i(m_Pos)), m_Size(new Vec2i(p_Size)),
	m_NormalSize(Vec2i(0, 0)), m_DrawCoord(Vec2i(0, 0)),
	m_DestinationDrawCoord(SDL_Rect{ m_Pos.x, m_Pos.y, p_Size.x, p_Size.y }),
	m_SourceDrawCoord(SDL_Rect{ 0, 0, p_Size.x, p_Size.y }),
	m_Angle(0.f),
	m_RotationCenter(SDL_Point{ p_Size.x/2, p_Size.y/2 }),
	m_Flip(SDL_RendererFlip::SDL_FLIP_NONE),
	Object(p_RegisterInHandler),
	m_ZOrder(0)
{
}

Drawable::~Drawable()
{
	FREE(m_Size);
	FREE(m_Position);
	if (m_Body != nullptr)
		FREE(m_Body);
	SDL_DestroyTexture(m_Texture);
}

void Drawable::setZOrder(int p_ZOrder)
{
	m_ZOrder = p_ZOrder;
	Drawable::SortZOrder();
}

int Drawable::getZOrder() const
{
	return m_ZOrder;
}

void Drawable::render(float dt)
{
	if (m_Parent != nullptr) {
		int px = (int)m_Parent->getPosition().x;
		int py = (int)m_Parent->getPosition().y;
		setPosition(Vec2i(px+m_Offset.x, py+m_Offset.y));
		//m_Angle = m_Parent->GetRotation();
	}
	SDL_RenderCopyEx(TrikytaEngine::getEngine()->getRenderer(), m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
}

void Drawable::attachTo(Drawable* obj, Vec2f p_Offset)
{
	Object::attachTo(obj);
	if (obj == nullptr) {
		Log("Error attempt to attach to null object!")
		return;
	}
	//followPosition(obj->getVecPosPtr());
	m_Offset = Vec2i((int)(p_Offset.x*m_Size->x), (int)(p_Offset.y*m_Size->y));
	SetRotationCenter(Vec2i(m_Size->x / 2, m_Size->y / 2));
}

void Drawable::attachTo(Physics2D::PhysicsBody* p_Phyobj, Vec2f p_Offset)
{
	Object::attachTo(p_Phyobj);
	if (p_Phyobj == nullptr) {
		Log("Error attempt to attach to null object!")
			return;
	}
	m_Offset = Vec2i((int)(p_Offset.x*m_Size->x), (int)(p_Offset.y*m_Size->y));
	//Vec2i((int)(m_Body->GetLocalCenter().x*PTM), int(m_Body->GetLocalCenter().y*PTM)));
	SetRotationCenter(Vec2i(m_Size->x / 2, m_Size->y / 2));
}

Physics2D::PhysicsBody* Drawable::Physicalize(Physics2D::BodyParams p_BodyParam,Physics2D::BodyType p_Type, Physics2D::BodyShape p_BodyShape=Physics2D::BodyShape::BOX, Vec2f p_Offset)
{
	using namespace Physics2D;
	auto size = Vec2i((int)(p_Offset.x*getSize().x), (int)(p_Offset.y*getSize().y));

	m_Body = PhysicsBody::CreateBody
	(
		PhysicsEngine::GetPhysicsWorld(), p_Type,
		p_BodyShape, p_BodyParam,
		Vec2f{ getPosition().x + (float)Utility::ToMeters(getSize().x), getPosition().y + (float)Utility::ToMeters(getSize().y) },
		std::vector<Vec2f>{
		Vec2f(
			(float)Utility::ToMeters(getSize().x - size.x),
			(float)Utility::ToMeters(getSize().y - size.y)
		)
	}
	);

	attachTo(m_Body, Vec2f(-0.5f, -0.5f));
	return m_Body;
}


void Drawable::SortZOrder()
{
	ObjectHandler::GetObjectHandler()->sort
	(
		[](Object* a, Object* b) 
		{ 
			Drawable* da = dynamic_cast<Drawable*>(a);
			Drawable* db = dynamic_cast<Drawable*>(b);
			if (da && db)
				return da->getZOrder() < db->getZOrder();
			return false;
		}
	);
}

Physics2D::PhysicsBody* Drawable::getBody()
{
	return m_Body;
}