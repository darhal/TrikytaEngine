#pragma once
#include <map>
#include <core/Objects/Object.h>

namespace Tmx {
	class Map;
	class Tileset;
}

struct TiledLayerData 
{
	TiledLayerData(int i, struct SDL_Rect* p_SourceDraw, struct SDL_Rect* p_DestDraw, struct SDL_Texture* p_Tex):
		SourceDraw(p_SourceDraw), 
		DestDraw(p_DestDraw), 
		Layer_Id(i),
		Tex(p_Tex)
	{}
	struct SDL_Rect* SourceDraw;
	struct SDL_Rect* DestDraw;
	struct SDL_Texture* Tex;
	int Layer_Id;
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
	// contain TiledLayerData indexed with layer index!
	std::vector<TiledLayerData>* m_LayerData;
private:
	bool LoadTilesets();
	void LoadLayers();
	friend class Tilesets;
	friend class ObjectGroup;
};