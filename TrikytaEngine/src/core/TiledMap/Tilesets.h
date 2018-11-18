#pragma once
#include <string>
#include <vector>
#include <map>
#include <SDL/SDL.h>
#include "core/Common/Vec2.h"
#include "core/Physics/PhysicsEngine.h"
#include "Parser/Tmx.h"
#include "TiledMap.h"

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
	Tmx::Object* m_Object;
};

struct TileAnimationData {
	TileAnimationData(int p_CurrentFrame, std::vector<std::pair<int, unsigned int>> p_GidAnimPair):
		m_CurrentFrame(p_CurrentFrame), m_GidAnimPairVec(p_GidAnimPair)
	{}
	TileAnimationData() :m_IsAnimated(false) {}
	int m_CurrentFrame;
	bool m_IsAnimated;
	std::vector<std::pair<int, unsigned int>> m_GidAnimPairVec;
};

struct TileData
{
	TileData(int p_GID, struct SDL_Rect* p_SourceDraw, struct SDL_Rect* p_DestDraw, struct SDL_Texture* p_Tex, bool p_IsAnimated, bool p_IsPhy) :
		GID(p_GID),
		SourceDraw(p_SourceDraw),
		DestDraw(p_DestDraw),
		Tex(p_Tex),
		IsPhy(p_IsPhy),
		isAnimated(p_IsAnimated)
	{}

	TileData(TileData& o) :
		Tex(o.Tex),
		IsPhy(o.IsPhy),
		isAnimated(o.isAnimated), 
		GID(o.GID),
		m_CurrentFrame(o.m_CurrentFrame),
		LastDeltaTime(o.LastDeltaTime),
		m_FramesVec(o.m_FramesVec)
	{
		SourceDraw = new SDL_Rect;
		DestDraw = new SDL_Rect;
		SourceDraw->x = o.SourceDraw->x;
		SourceDraw->y = o.SourceDraw->y;
		SourceDraw->w = o.SourceDraw->w;
		SourceDraw->h = o.SourceDraw->h;
		DestDraw->x = o.DestDraw->x;
		DestDraw->y = o.DestDraw->y;
		DestDraw->w = o.DestDraw->w;
		DestDraw->h = o.DestDraw->h;
	}
	struct SDL_Rect* SourceDraw;
	struct SDL_Rect* DestDraw;
	struct SDL_Texture* Tex;
	int GID;
	bool isAnimated;
	std::vector<TileData*> m_FramesVec;
	unsigned int m_CurrentFrame; 
	unsigned int m_AnimationDuration;
	float LastDeltaTime;
	std::vector<Physics2D::PhysicsBody*>* PhyBodys = nullptr;
	bool IsPhy;

	void setPosition(Vec2i pos, class TiledMap* p_Map) {
		int YAdjuster = p_Map->getMap()->GetTileHeight() - SourceDraw->h; // adjust the Y to fit in the grids!
		DestDraw->x = pos.x * (p_Map->getMap()->GetTileWidth()) + p_Map->m_Position.x;
		DestDraw->y = pos.y * p_Map->getMap()->GetTileHeight() + p_Map->m_Position.y + YAdjuster;
	}
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
	std::map<int, TileData*> m_Tiles;
	//std::map<int, std::vector<TilesetObjectData*>> m_TileObjects;
	std::map<int, std::vector<Physics2D::PhysicsBody*>> m_TileObjects;
	std::map<int, std::vector<std::pair<int, unsigned int>>> m_TileAnimations;
	SDL_Texture* m_ImageTexture;
	Vec2i m_ImageSize;
	Vec2i m_TileSize;
	int m_FirstGid;
	int m_TileIndex;
	friend struct TileData;
};