#pragma once

namespace Tmx {
	class Map;
	class Tileset;
	class Tile;
	class Object;
}

class ObjectGroup
{
public:
	ObjectGroup(Tmx::Map*);
	void ParseGroups(Tmx::Map*);
	void ProcessPhysicalizedObject(const Tmx::Object*);
	~ObjectGroup();
private:
};