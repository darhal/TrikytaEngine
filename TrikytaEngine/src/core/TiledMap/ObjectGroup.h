#pragma once
#include <vector>
#include <unordered_map>

namespace Tmx {
	class Map;
	class Tileset;
	class Tile;
	class Object;
	class Text;
	class ObjectGroup;
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
	void ProcessText(const Tmx::Text*, const Tmx::Object*);
	std::vector<Physics2D::PhysicsBody*> getBodies() const {return m_Bodies;}
	std::vector<Drawable*> getDrawables() const { return m_Drawables; }
	const Tmx::ObjectGroup* getObjectGroup(const std::string&);
	Physics2D::PhysicsBody* getBodyByName(const std::string& p_Name);
	Physics2D::PhysicsBody* getBodyByID(int p_gid);
	static void GetPhysicsSettings(const Tmx::Object* p_Object, Physics2D::BodyType& p_type, Physics2D::BodyParams& p_params);
	~ObjectGroup();
private:
	std::vector<Physics2D::PhysicsBody*> m_Bodies;
	std::vector<Drawable*> m_Drawables;
	std::unordered_map<std::string, Physics2D::PhysicsBody*> m_ObjectsByName;
	std::unordered_map<int, Physics2D::PhysicsBody*> m_ObjectsByID;
	Tmx::Map* m_Map;
};