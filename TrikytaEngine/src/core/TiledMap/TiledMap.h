#pragma once
#include <map>
#include <cstring>
#include <core/Drawable/Drawable.h>
#include "core/Common/defines.h"
#include "core/TiledMap/ObjectGroup.h"
#include "core/Components/Component.h"

struct TileData;
class Tilesets;

namespace Tmx {
	class Map;
	class Tileset;
	class TileLayer;
}

enum class LayerType{
	IMMEDIATE,
	RETAINED
};

ADD_COMPONENT(TILE_COMPONENT)
struct LayerData : public Component
{
	REGISTER_COMPONENT(LayerData, TILE_COMPONENT);
	LayerData(int layerId, TileData* tiledLayerData):
		layerId(layerId), tiledLayerData(tiledLayerData)
	{}

	~LayerData();
	//LayerData(const LayerData& o) : layerId(o.layerId), tiledLayerData(o.tiledLayerData){}
	int layerId;
	TileData* tiledLayerData;
};

struct MapPart
{
	MapPart(SDL_Texture* p_Texture, SDL_Rect p_Coords, Vec2i p_InitPos)
		: m_Texture(p_Texture)
	{
		memcpy(&m_Coords, &p_Coords, sizeof(SDL_Rect));
		memcpy(&m_InitPos, &p_InitPos, sizeof(Vec2i));
	}
	SDL_Texture* m_Texture;
	SDL_Rect m_Coords;
	Vec2i m_InitPos;
};

class TiledMap: public Drawable
{
public:
	static TiledMap* Create(std::string, bool=true);

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
	virtual void setAffectedByCamera(Camera* cam) override;
	Vec2i getPosition() override;

	inline ObjectGroup& getGroupManager() 
	{
		return m_Group;
	}

	bool isBodyPartOfTileset(Physics2D::PhysicsBody* body, const std::string& tilsetName, int id);

	void deleteTileInLayer(LayerData* tileToDelete);
	LayerData* getGridInLayerAt(const std::string&, int, int);
	void addTileToLayer(Tilesets* tile_set, int id, const std::string& layer_name, Vec2i pos);
	Tilesets* getTilset(const std::string& tileset_name);
protected:
	TiledMap(Tmx::Map*, std::string&, bool=true);
private:
	Tmx::Map* m_Map;
	std::string m_AssetsPath;
	std::vector<Tilesets>* m_MapTilesets;
	std::vector<LayerData*> m_cachedImmediateTiles;
	// contain TileData indexed with layer index!
	std::vector<LayerData*> m_LayerData;
	std::map<std::pair<std::string, int>, LayerData*> m_ImmediateLayerData;
	std::map<std::string, Tilesets*> m_MapTilesetsByName;
	std::vector<MapPart> m_MapGrids;
	//std::map<const std::string&, std::pair<const Tmx::TileLayer*, int>> m_LayerByName;
	std::map<std::string, std::pair<const Tmx::TileLayer*, int>> m_LayerByName;
	ObjectGroup m_Group;
private:
	bool LoadTilesets();
	void LoadLayers();
	void LoadMapIntoTexture();
	void DivideMap(Vec2i&);
	friend class Tilesets;
	friend class ObjectGroup;
	friend struct TileData;
};
