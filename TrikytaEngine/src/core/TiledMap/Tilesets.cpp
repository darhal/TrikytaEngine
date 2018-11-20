#include <sstream>
#include "Tilesets.h"
#include <SDL/SDL_image.h>
#include "TiledMap.h"
#include "Parser/Tmx.h"
#include "core/Common/defines.h"
#include "core/Common/EngineInstance.h"
#include "core/Common/TrikytaEngine.h"
#include "core/Physics/Fixture.h"

Tilesets::Tilesets(TiledMap* p_Map, int i): 
	m_Tileset(*p_Map->m_Map->GetTileset(i)), 
	m_TilesetName(p_Map->m_Map->GetTileset(i)->GetName()),
	m_TileIndex(i)
{

	LoadTilesets(p_Map);
}

Tilesets::~Tilesets() 
{
	for (auto data : m_Tiles) {
		FREE(data.second->SourceDraw);
		FREE(data.second->DestDraw);
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
	//if (m_Tileset.GetTiles().size() < 0) { return; }
	for (int gid = m_FirstGid; gid <= m_Lastgid; gid++) {
		auto currentTile = m_Tileset.GetTile(_id - 1);
		if (currentTile != NULL) {
			if (currentTile->HasObjects()) {
				ProcessTileObjects(currentTile, gid); //PROCESS COLLISION!
			}
			if (currentTile->IsAnimated()) {
				Tilesets::ProcessTileAnimation(currentTile, gid);
			}
		}
		if (_id % m_TileAmountWidth == 1) {
			y = ((m_TileSize.y+spacing) * ((_id / m_TileAmountWidth) - 1)) + (m_TileSize.y+ spacing)+ spacing;
		}
		x = (m_TileSize.x+ margin) * ((_id % m_TileAmountWidth ? _id % m_TileAmountWidth : m_TileAmountWidth)-1)+ margin;
		int Tile_SizeX = m_TileSize.x;
		int Tile_SizeY = m_TileSize.y;
		int YAdjuster = p_Map->getMap()->GetTileHeight() - Tile_SizeY; // adjust the Y to fit in the grids!
		int map_x = 0, map_y = 0;
		SDL_Rect* m_SourceDrawCoord = new SDL_Rect{
			x,
			y,
			Tile_SizeX,
			Tile_SizeY
		};
		SDL_Rect* m_DestinationDrawCoord = new SDL_Rect{
			int((map_x*(p_Map->getMap()->GetTileWidth()) + p_Map->m_Position.x)),
			int((map_y*p_Map->getMap()->GetTileHeight() + p_Map->m_Position.y + YAdjuster)),
			int(Tile_SizeX),
			int(Tile_SizeY)
		};
		m_Tiles[gid] = new TileData(gid, m_SourceDrawCoord, m_DestinationDrawCoord, m_ImageTexture, false, false);
		m_Tiles[gid]->isAnimated = false;
		m_Tiles[gid]->TileName = m_Tileset.GetName();
		m_Tiles[gid]->id = _id;
		_id++;
	}
	for (auto animTiles : m_TileAnimations) {
		m_Tiles[animTiles.first]->isAnimated = true;
		m_Tiles[animTiles.first]->m_FramesVec.reserve(animTiles.second.size());
		m_Tiles[animTiles.first]->m_CurrentFrame = 0;
		m_Tiles[animTiles.first]->LastDeltaTime = 0;
		for (auto animFrameGIDs : animTiles.second) {
			m_Tiles[animFrameGIDs.first]->m_AnimationDuration = animFrameGIDs.second; // this is animation time
			m_Tiles[animTiles.first]->m_FramesVec.push_back(m_Tiles[animFrameGIDs.first]);
		}
	}
}

void Tilesets::ProcessTileAnimation(const Tmx::Tile* p_Tile,const int& gid)
{
	if (p_Tile->IsAnimated())
	{
		//printf(
		//	"Tile is animated: %d frames with total duration of %dms.\n",
		//	p_Tile->GetFrameCount(), p_Tile->GetTotalDuration());
		const std::vector<Tmx::AnimationFrame> &frames = p_Tile->GetFrames();
		for (std::vector<Tmx::AnimationFrame>::const_iterator it =
			frames.begin(); it != frames.end(); it++)
		{
			//printf("\tFrame Tile ID = %d, Duration = %dms\n",
			//	it->GetTileID(), it->GetDuration());
			int tileGid = it->GetTileID() + m_FirstGid;
			m_TileAnimations[gid].push_back(std::make_pair(tileGid, it->GetDuration()));
		}
	}
}

void Tilesets::ProcessTileObjects(const Tmx::Tile* p_Tile, const int& gid)
{
	if (p_Tile->HasObjects()) ///PROCESS COLLISION!
	{
		// Iterate through all Collision objects in the p_Tile.
		m_TileObjects[gid].reserve(p_Tile->GetNumObjects());
		for (int j = 0; j < p_Tile->GetNumObjects(); ++j)
		{
			// Get an object.
			const Tmx::Object *object = p_Tile->GetObject(j);

			if (object->GetHeight() == 0 && object->GetWidth() == 0) {
				// Print Polygon points.
				const Tmx::Polygon *polygon = object->GetPolygon();
				if (polygon != 0)
				{
					std::vector<Vec2f> polyBufferPoints;
					polyBufferPoints.reserve(polygon->GetNumPoints() + 1);
					for (int i = 0; i < polygon->GetNumPoints(); i++)
					{
						const Tmx::Point &point = polygon->GetPoint(i);
						polyBufferPoints.emplace_back(point.x, point.y);
					}
					polyBufferPoints.emplace_back(polygon->GetPoint(0).x, polygon->GetPoint(0).y);
					m_TileObjects[gid].emplace_back(TilesetObjectData{ Vec2f((float)object->GetX(), (float)object->GetY()), polyBufferPoints, Physics2D::BodyShape::POLYGON, Physics2D::BodyType::STATIC });
				}
				// Print Polyline points.
				const Tmx::Polyline *polyline = object->GetPolyline();
				if (polyline != 0)
				{
					std::vector<Vec2f> polyBufferPoints;
					polyBufferPoints.reserve(polyline->GetNumPoints() + 1);
					for (int i = 0; i < polyline->GetNumPoints(); i++)
					{
						const Tmx::Point &point = polyline->GetPoint(i);
						polyBufferPoints.emplace_back(point.x, point.y);

					}
					polyBufferPoints.emplace_back(polyline->GetPoint(0).x, polyline->GetPoint(0).y);
					m_TileObjects[gid].emplace_back(TilesetObjectData{ Vec2f((float)object->GetX(), (float)object->GetY()), polyBufferPoints, Physics2D::BodyShape::POLYGON, Physics2D::BodyType::STATIC });
				}
			}
			else {
				auto vecPos = Vec2f((float)object->GetX() + object->GetWidth() / PTM, (float)object->GetY() + object->GetHeight() / PTM);
				auto vecCoord = std::vector<Vec2f>{ Vec2f(object->GetWidth() / PTM, object->GetHeight() / PTM) };
				if (object->GetType() == "circle") {
					m_TileObjects[gid].emplace_back(TilesetObjectData{ vecPos, vecCoord, Physics2D::BodyShape::CIRCLE, Physics2D::BodyType::STATIC });
				}else{
					m_TileObjects[gid].emplace_back(TilesetObjectData{ vecPos, vecCoord, Physics2D::BodyShape::BOX, Physics2D::BodyType::STATIC });
				}
			}
		}
	}
}


/*void Tilesets::ProcessTileObjects(const Tmx::Tile* p_Tile,const int& gid)
{
	if (p_Tile->HasObjects()) ///PROCESS COLLISION!
	{
		// Iterate through all Collision objects in the p_Tile.
		m_TileObjects[gid].reserve(p_Tile->GetNumObjects());
		for (int j = 0; j < p_Tile->GetNumObjects(); ++j)
		{
			Physics2D::PhysicsBody* body = nullptr;
			const Tmx::Object* object = p_Tile->GetObject(j);// Get an object.
			Physics2D::BodyType b_type = Physics2D::BodyType::STATIC;
			Physics2D::BodyParams b_params;
			ObjectGroup::GetPhysicsSettings(object, b_type, b_params);
			if (object->GetHeight() == 0 && object->GetWidth() == 0) {
				// Print Polygon points.
				const Tmx::Polygon* polygon = object->GetPolygon();
				if (polygon != 0)
				{
					std::vector<Vec2f> polyBufferPoints;
					polyBufferPoints.reserve(polygon->GetNumPoints() + 1);
					for (int i = 0; i < polygon->GetNumPoints(); i++)
					{
						const Tmx::Point &point = polygon->GetPoint(i);
						polyBufferPoints.emplace_back(point.x, point.y);
					}
					polyBufferPoints.emplace_back(polygon->GetPoint(0).x, polygon->GetPoint(0).y);
					if (body == nullptr) {
						LogTerminal("Creating polygone!");
						body = Physics2D::PhysicsBody::CreateBody
						(
							Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type, Physics2D::BodyShape::POLYGON, b_params, Vec2f((float)object->GetX(), (float)object->GetY()),
							polyBufferPoints
						);
					}else{
						Physics2D::FixtureDef fixture{ b_type, Physics2D::BodyShape::POLYGON, b_params, Vec2f((float)object->GetX(), (float)object->GetY()) , polyBufferPoints };
						body->CreateFixture(&fixture);
					}
				}
				// Print Polyline points.
				const Tmx::Polyline *polyline = object->GetPolyline();
				if (polyline != 0)
				{
					std::vector<Vec2f> polyBufferPoints;
					polyBufferPoints.reserve(polyline->GetNumPoints() + 1);
					for (int i = 0; i < polyline->GetNumPoints(); i++)
					{
						const Tmx::Point &point = polyline->GetPoint(i);
						polyBufferPoints.emplace_back(point.x, point.y);

					}
					polyBufferPoints.emplace_back(polyline->GetPoint(0).x, polyline->GetPoint(0).y);
					if (body == nullptr) {
						body = Physics2D::PhysicsBody::CreateBody
						(
							Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,Physics2D::BodyShape::POLYGON, b_params,Vec2f((float)object->GetX(), (float)object->GetY()),
							polyBufferPoints
						);
					}else{
						Physics2D::FixtureDef fixture{ b_type, Physics2D::BodyShape::POLYGON, b_params, Vec2f((float)object->GetX(), (float)object->GetY()) , polyBufferPoints };
						body->CreateFixture(&fixture);
					}
				}
			}else {
				auto vecPos = Vec2f((float)object->GetX() + object->GetWidth() / PTM, (float)object->GetY() + object->GetHeight() / PTM);
				auto vecCoord = std::vector<Vec2f>{ Vec2f(object->GetWidth() / PTM, object->GetHeight() / PTM) };
				if (object->GetType() == "circle") {
					if (body == nullptr) {
						body =
							Physics2D::PhysicsBody::CreateBody(Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,Physics2D::BodyShape::CIRCLE, b_params,vecPos,vecCoord);
					}else {
						Physics2D::FixtureDef fixture{ b_type, Physics2D::BodyShape::CIRCLE, b_params, vecPos, vecCoord };
						body->CreateFixture(&fixture);
					}
				}else{
					if (body == nullptr) {
						body =
							Physics2D::PhysicsBody::CreateBody(Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,Physics2D::BodyShape::BOX, b_params,vecPos,vecCoord);
					}else {
						Physics2D::FixtureDef fixture{ b_type, Physics2D::BodyShape::BOX, b_params, vecPos, vecCoord };
						body->CreateFixture(&fixture);
					}
				}
			}
			m_TileObjects[gid].emplace_back(body);
		}
	}
}*/