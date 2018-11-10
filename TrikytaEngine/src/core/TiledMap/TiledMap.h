#pragma once
#include <map>
#include <core/Drawable/Drawable.h>
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

struct  MapPart
{
	MapPart(SDL_Texture* p_Texture, SDL_Rect p_Coords) 
		: m_Texture(p_Texture), m_Coords(p_Coords)
	{}
	SDL_Texture* m_Texture;
	SDL_Rect m_Coords;
};

class TiledMap: public Drawable
{
public:
	static TiledMap* Create(std::string);

	bool init();
	virtual void render(float) override;
	void renderAnimations(float);
	Tmx::Map* getMap() { return m_Map; }
	~TiledMap();
	void setPosition(Vec2i);
	void translateMap(Vec2i);
	void AddPhysicsDebugDrawToMapTexture();
	inline virtual void updateRenderPositionFromCamera(Vec2i p_pos) override
	{
		setPosition(p_pos);
	}

	Vec2i getPosition() override;
protected:
	TiledMap(Tmx::Map*, std::string&);
private:
	Tmx::Map* m_Map;
	std::string m_AssetsPath;
	std::vector<class Tilesets>* m_MapTilesets;
	std::vector<Physics2D::PhysicsBody*> m_allMapBodies;
	std::vector<LayerData*> m_cachedAnimatiedTiles;
	ObjectGroup m_Group;
	// contain TileData indexed with layer index!
	std::vector<LayerData> m_LayerData;
	std::vector<MapPart> m_MapGrids;
private:
	bool LoadTilesets();
	void LoadLayers();
	void LoadMapIntoTexture();
	void DivideMap(Vec2i&);
	friend class Tilesets;
	friend class ObjectGroup;
	friend struct TileData;
};