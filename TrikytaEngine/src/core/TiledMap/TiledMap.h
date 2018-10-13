#pragma once
#include <map>
#include <core/Objects/Object.h>
#include "core/Common/defines.h"

namespace Tmx {
	class Map;
	class Tileset;
}

struct LayerData
{
	LayerData(int layerId, struct TileData* tiledLayerData):
		layerId(layerId), tiledLayerData(tiledLayerData)
	{}
	int layerId;
	struct TileData* tiledLayerData;
};

class TiledMap: public Object
{
public:
	static TiledMap* Create(std::string);

	bool init();
	virtual void render(float);
	Tmx::Map* getMap() { return m_Map; }
	~TiledMap();

	bool isReady;
protected:
	TiledMap(Tmx::Map*, std::string&);
private:
	Vec2i m_Position;
	Tmx::Map* m_Map;
	std::string m_AssetsPath;
	std::vector<class Tilesets>* m_MapTilesets;
	// contain TileData indexed with layer index!
	std::vector<LayerData>* m_LayerData;
private:
	bool LoadTilesets();
	void LoadLayers();
	friend class Tilesets;
	friend class ObjectGroup;
	friend struct TileData;
};