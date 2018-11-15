#pragma once
#include <vector>

namespace Tmx {
	class Map;
	class Tileset;
	class Tile;
	class Object;
}
namespace Physics2D
{
	class PhysicsBody;
	enum class BodyType;
	struct BodyParams;
}

class Drawable;

class ObjectGroup
{
public:
	ObjectGroup(Tmx::Map*);
	void ParseGroups(Tmx::Map*);
	void ProcessPhysicalizedObject(const Tmx::Object*);
	std::vector<Physics2D::PhysicsBody*> getBodies() const {return m_Bodies;}
	std::vector<Drawable*> getDrawables() const { return m_Drawables; }
	~ObjectGroup();
private:
	void GetPhysicsSettings(const Tmx::Object* p_Object, Physics2D::BodyType& p_type, Physics2D::BodyParams& p_params);
	std::vector<Physics2D::PhysicsBody*> m_Bodies;
	std::vector<Drawable*> m_Drawables;
};