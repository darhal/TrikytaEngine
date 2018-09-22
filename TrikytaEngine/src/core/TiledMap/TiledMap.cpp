#include "TiledMap.h"
#include "Tilesets.h"
#include "core/Common/TrikytaEngine.h"
#include "Parser/Tmx.h"
#include "core/Common/Vec2.h"
#include "core/Common/defines.h"

TiledMap* TiledMap::Create(std::string p_Filename) 
{
	Tmx::Map* map = new Tmx::Map();
	map->ParseFile(p_Filename);
	if (map->HasError())
	{
		Log("ERROR: Loading Tiled Map failed with code: %d\n", map->GetErrorCode());
		Log("ERROR: Loading Tiled Map failed with code text: %s\n", map->GetErrorText().c_str());
		delete map; // delete and return null ptr!
		return nullptr;
	}
	// Find assets path:
	auto const pos = p_Filename.find_last_of('/');
	p_Filename = p_Filename.substr(0, pos+1);
	return new TiledMap(map, p_Filename);
}

TiledMap::TiledMap(Tmx::Map* p_Map,std::string p_AssetsPath) : m_Map(p_Map), m_AssetsPath(p_AssetsPath)
{
	TiledMap::init();
}

TiledMap::~TiledMap()
{
	for (auto& itr : *m_LayerData) {
		FREE(itr.DestDraw);
		FREE(itr.SourceDraw);
		SDL_DestroyTexture(itr.Tex);
	}
	FREE(m_LayerData);
	FREE(m_MapTilesets);
	FREE(m_Map);
}

bool TiledMap::init() 
{
	isReady = false;
	m_Position = Vec2i(0,0);
	TiledMap::LoadTilesets();
	LoadThread = new std::thread(&TiledMap::LoadLayers, this);
	return true;
}

void TiledMap::render(float dt) 
{
	if (!isReady) { return; }
	auto r = ENGINE->getRenderer();
	for (auto& itr : *m_LayerData) {
		SDL_RenderCopy(r, itr.Tex, itr.SourceDraw, itr.DestDraw);
	}
}

bool TiledMap::LoadTilesets()
{
	// Iterate through the tilesets.
	m_MapTilesets = new std::vector<Tilesets>;
	m_MapTilesets->reserve(m_Map->GetNumTilesets());
	for (int i = 0; i < m_Map->GetNumTilesets(); ++i)
	{
		m_MapTilesets->emplace_back(this, i);
		//Log("[TILEDMAPS] Tilesets ID: %d || Tileset name : %s", i, m_Map->GetTileset(i)->GetName().c_str())
	}
	return true;
}

void TiledMap::LoadLayers()
{
	int ind = 0;
	m_LayerData = new std::vector<TiledLayerData>;
	for (int LayerIndex = 0; LayerIndex < m_Map->GetNumTileLayers(); ++LayerIndex)
	{
		const Tmx::TileLayer* tileLayer = m_Map->GetTileLayer(LayerIndex);
		Log("[TILED MAPS] Layer: %s (W: %d, H: %d)", tileLayer->GetName().c_str(), tileLayer->GetWidth(), tileLayer->GetHeight())
		for (int y = 0; y < tileLayer->GetHeight(); ++y)
		{
			for (int x = 0; x < tileLayer->GetWidth(); ++x)
			{
				if (tileLayer->GetTileTilesetIndex(x, y) != -1)
				{
					m_LayerData->reserve(m_LayerData->size() + 1);
					ind++;
					int TilesetIndex = tileLayer->GetTileTilesetIndex(x, y);
					Uint32 Gid = tileLayer->GetTileGid(x, y);
					auto TilesetData = &(m_MapTilesets->at(TilesetIndex));
					int Tile_SizeX = TilesetData->m_TileSize.x;
					int Tile_SizeY = TilesetData->m_TileSize.y;
					SDL_Rect* m_SourceDrawCoord = new SDL_Rect{
						TilesetData->m_TilesPos[Gid]->x,
						TilesetData->m_TilesPos[Gid]->y,
						Tile_SizeX,
						Tile_SizeY
					};
					SDL_Rect* m_DestinationDrawCoord = new SDL_Rect {
						x*Tile_SizeX + m_Position.x,
						y*Tile_SizeY + m_Position.y,
						Tile_SizeX,
						Tile_SizeY
					};
					m_LayerData->emplace_back(LayerIndex, m_SourceDrawCoord, m_DestinationDrawCoord, m_MapTilesets->at(TilesetIndex).m_ImageTexture);
				}
			}
		}
	}
	isReady = true;
	Log("[TILEDMAPS] Gonna be %d Calls in render!", ind)
}