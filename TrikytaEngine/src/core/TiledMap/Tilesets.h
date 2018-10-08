#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include "core/Common/Vec2.h"
#include "core/Physics/PhysicsEngine.h"

namespace Tmx {
	class Map;
	class Tileset;
	class Tile;
}

struct TilesetObjectData {
	Vec2f m_ObjectPos;
	std::vector<Vec2f> m_ObjectCoord;
	Physics2D::BodyShape m_BodyShape;
	Physics2D::BodyType m_BodyType;
	Physics2D::BodyParams m_BodyParams;
};

class Tilesets 
{
public:
	Tilesets(class TiledMap*, int);
	~Tilesets();
protected:

private:
	void LoadTilesets(class TiledMap*);
	void LoadTiles(std::string, class TiledMap*);
	bool LoadImage(std::string, class TiledMap*);
	void ProcessTileObjects(const Tmx::Tile*,const int&);
	void ProcessTileAnimation(const Tmx::Tile*,const int&);
	friend class TiledMap;
private:
	const std::string m_TilesetName;
	const Tmx::Tileset& m_Tileset;
	std::map<int, Vec2i*> m_TilesPos;
	std::map<int, std::vector<TilesetObjectData*>> m_TileObjects;
	std::map<int, std::vector<std::pair<int, int>>> m_TileAnimData;
	SDL_Texture* m_ImageTexture;
	Vec2i m_ImageSize;
	Vec2i m_TileSize;
	int m_FirstGid;
	int m_TileIndex;
};