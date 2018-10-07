#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include "core/Common/Vec2.h"

namespace Tmx {
	class Map;
	class Tileset;
	class Tile;
}

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
	void ProcessTile(const Tmx::Tile*);
	friend class TiledMap;
private:
	const std::string m_TilesetName;
	const Tmx::Tileset& m_Tileset;
	std::map<int, Vec2i*> m_TilesPos;
	SDL_Texture* m_ImageTexture;
	Vec2i m_ImageSize;
	Vec2i m_TileSize;
	int m_FirstGid;
	int m_TileIndex;
};