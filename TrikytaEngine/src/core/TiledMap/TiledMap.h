#pragma once
#include <map>
#include <core/Objects/Object.h>
#include "core/Common/defines.h"
#include "core/TiledMap/ObjectGroup.h"

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
	void setPosition(Vec2i);
	void translateMap(Vec2i);

	Vec2i getPosition() override;
	inline Vec2i getSize() { return Vec2i(m_MapDst.w, m_MapDst.h); }
	bool isReady;
protected:
	TiledMap(Tmx::Map*, std::string&);
private:
	Vec2i m_Position;
	Tmx::Map* m_Map;
	std::string m_AssetsPath;
	std::vector<class Tilesets>* m_MapTilesets;
	SDL_Texture* m_MapTexture;
	SDL_Rect m_MapDst;
	SDL_Rect m_MapSrc;
	std::vector<Physics2D::PhysicsBody*> m_allMapBodies;
	ObjectGroup m_Group;
	// contain TileData indexed with layer index!
	std::vector<LayerData>* m_LayerData;
private:
	bool LoadTilesets();
	void LoadLayers();
	friend class Tilesets;
	friend class ObjectGroup;
	friend struct TileData;
};