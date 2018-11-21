#pragma once
#include <map>
#include <unordered_map>
#include <cstring>
#include <core/Drawable/Drawable.h>
#include "core/Common/defines.h"
#include "core/TiledMap/ObjectGroup.h"

namespace Tmx {
	class Map;
	class Tileset;
}

enum class LayerType{
	IMMEDIATE,
	RETAINED
};

struct LayerData
{
	LayerData(int layerId, struct TileData* tiledLayerData):
		layerId(layerId), tiledLayerData(tiledLayerData)
	{}
	int layerId;
	struct TileData* tiledLayerData;
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

struct ImmediateLayer : public LayerData
{
	ImmediateLayer(int layerId, struct TileData* tiledLayerData, class Drawable* drble) : LayerData(layerId, tiledLayerData), m_drble(drble)
	{}
	class Drawable* m_drble;
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
	virtual void setAffectedByCamera(Camera* cam) override;
	Vec2i getPosition() override;

	inline ObjectGroup& getGroupManager() 
	{
		return m_Group;
	}

	const std::vector<Physics2D::PhysicsBody*>& getTilesetBodiesByID(const std::string& tilsetName, int id);
	bool isBodyPartOfTileset(Physics2D::PhysicsBody* body, const std::string& tilsetName, int id);
protected:
	TiledMap(Tmx::Map*, std::string&);
private:
	Tmx::Map* m_Map;
	std::string m_AssetsPath;
	std::vector<class Tilesets>* m_MapTilesets;
	std::map<std::string, std::map<int, std::vector<Physics2D::PhysicsBody*>>> m_BodyByTile;
	//std::map<std::pair<const std::string&, int>, std::vector<Physics2D::PhysicsBody*>> m_BodyByTile;
	std::vector<Physics2D::PhysicsBody*> m_allMapBodies;
	std::vector<LayerData*> m_cachedAnimatiedTiles;
	ObjectGroup m_Group;
	// contain TileData indexed with layer index!
	std::vector<LayerData> m_LayerData;
	std::map<std::pair<const std::string&, int>, ImmediateLayer> m_ImmediateLayerData;
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

