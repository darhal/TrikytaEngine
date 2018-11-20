#pragma once
#include <vector>
#include <unordered_map>
#include <string>

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
	void ProcessPhysicalizedObject(const Tmx::Object*, const Tmx::ObjectGroup*);
	void ProcessText(const Tmx::Text*, const Tmx::Object*);
	std::vector<Physics2D::PhysicsBody*> getBodies() const {return m_Bodies;}
	std::vector<Drawable*> getDrawables() const { return m_Drawables; }
	const Tmx::ObjectGroup* getObjectGroup(const std::string&);

	const std::vector<Physics2D::PhysicsBody*>& getBodiesByName(const std::string& p_Name);
	Physics2D::PhysicsBody* getBodyByID(int p_gid);
	inline std::vector<Physics2D::PhysicsBody*>& getBodiesByLayer(std::string& layerName) {return m_BodyByLayer[layerName];}
	bool isBodyInLayer(Physics2D::PhysicsBody* p_Body, const std::string& layerName);
	bool isBodyInGroupByName(Physics2D::PhysicsBody* p_Body, const std::string& p_Name);

	static void GetPhysicsSettings(const Tmx::Object* p_Object, Physics2D::BodyType& p_type, Physics2D::BodyParams& p_params);
	~ObjectGroup();
private:
	std::vector<Physics2D::PhysicsBody*> m_Bodies;
	std::vector<Drawable*> m_Drawables;
	std::unordered_map<std::string, std::vector<Physics2D::PhysicsBody*>> m_ObjectsByName;
	std::unordered_map<int, Physics2D::PhysicsBody*> m_ObjectsByID;
	std::unordered_map<std::string, const Tmx::ObjectGroup*> m_ObjectLayer;
	std::unordered_map<std::string, std::vector<Physics2D::PhysicsBody*>> m_BodyByLayer;
	Tmx::Map* m_Map;
};