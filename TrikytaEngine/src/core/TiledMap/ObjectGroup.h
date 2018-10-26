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
}
class ObjectGroup
{
public:
	ObjectGroup(Tmx::Map*);
	void ParseGroups(Tmx::Map*);
	void ProcessPhysicalizedObject(const Tmx::Object*);
	std::vector<Physics2D::PhysicsBody*> getBodies() const {return m_Bodies;}
	~ObjectGroup();
private:
	std::vector<Physics2D::PhysicsBody*> m_Bodies;
};