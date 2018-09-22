#include <sstream>
#include "Tilesets.h"
#include <SDL/SDL_image.h>
#include "TiledMap.h"
#include "Parser/Tmx.h"
#include "core/Common/defines.h"
#include "core/Common/EngineInstance.h"
#include "core/Common/TrikytaEngine.h"

Tilesets::Tilesets(TiledMap* p_Map, int i): 
	m_Tileset(*p_Map->m_Map->GetTileset(i)), 
	m_TilesetName(p_Map->m_Map->GetTileset(i)->GetName()),
	m_TileIndex(i)
{

	LoadTilesets(p_Map);
}

Tilesets::~Tilesets() 
{
	for (auto const& data : m_TilesPos) {
		FREE(data.second);
	}
	SDL_DestroyTexture(m_ImageTexture);
}

void Tilesets::LoadTilesets(TiledMap* p_Map)
{
	m_ImageSize = Vec2i(m_Tileset.GetImage()->GetWidth(), m_Tileset.GetImage()->GetHeight());
	m_FirstGid = m_Tileset.GetFirstGid();
	m_TileSize = Vec2i(m_Tileset.GetTileWidth(), m_Tileset.GetTileHeight());
	if (LoadImage(m_Tileset.GetImage()->GetSource(), p_Map)) {
		LoadTiles(m_Tileset.GetImage()->GetSource(), p_Map);
	}
}

bool Tilesets::LoadImage(std::string m_ImgSource, TiledMap* p_Map)
{
	m_ImgSource = p_Map->m_AssetsPath + m_ImgSource;
	SDL_Surface* TempSurface = IMG_Load(m_ImgSource.c_str());
	if (TempSurface == NULL) {
		Log("[TILEDMAP] Unable to load image : %s : %s", m_ImgSource.c_str(), IMG_GetError());
		return false;
	}

	// Convert SDL surface to a texture
	if ((m_ImageTexture = SDL_CreateTextureFromSurface(ENGINE->getRenderer(), TempSurface)) == NULL) {
		Log("[TILEDMAP] Unable to create SDL Texture : %s : %s", m_ImgSource.c_str(), IMG_GetError());
		return false;
	}

	// Grab dimensions too m_ImageSize!
	SDL_QueryTexture(m_ImageTexture, NULL, NULL, &m_ImageSize.x, &m_ImageSize.y);
	SDL_FreeSurface(TempSurface);
	return true;
}

void Tilesets::LoadTiles(std::string m_ImgSource, TiledMap* p_Map)
{
	m_ImgSource = p_Map->m_AssetsPath + m_ImgSource;
	Log("%s tiles: GID: %d", m_ImgSource.c_str(), m_FirstGid);
	int m_TileAmountWidth = (int)(floor(m_ImageSize.x / m_TileSize.x));
	int m_Lastgid = (int)(m_TileAmountWidth * floor(m_ImageSize.y / m_TileSize.y) + m_FirstGid - 1);
	int margin = m_Tileset.GetMargin(), spacing = m_Tileset.GetSpacing();
	int x = 0, y = 0;
	int _id = 1;
	for (int gid = m_FirstGid; gid <= m_Lastgid; gid++) {
		if (_id % m_TileAmountWidth == 1) {
			y = ((m_TileSize.y+spacing) * ((_id / m_TileAmountWidth) - 1)) + (m_TileSize.y+ spacing)+ spacing;
		}
		x = (m_TileSize.x+ margin) * ((_id % m_TileAmountWidth ? _id % m_TileAmountWidth : m_TileAmountWidth)-1)+ margin;
		_id++;
		m_TilesPos[gid] = new Vec2i(x, y);
		//Log("Pos (%d, %d):", x, y);
	}
}