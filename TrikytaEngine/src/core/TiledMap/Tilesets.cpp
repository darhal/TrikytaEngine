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
	for (auto& data : m_TilesPos) {
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
	//if (m_Tileset.GetTiles().size() < 0) { return; }
	for (int gid = m_FirstGid; gid <= m_Lastgid; gid++) {
		if (m_Tileset.GetTile(_id - 1) != NULL) {
			ProcessTile(m_Tileset.GetTile(_id - 1), gid); //PROCESS COLLISION!
		}
		if (_id % m_TileAmountWidth == 1) {
			y = ((m_TileSize.y+spacing) * ((_id / m_TileAmountWidth) - 1)) + (m_TileSize.y+ spacing)+ spacing;
		}
		x = (m_TileSize.x+ margin) * ((_id % m_TileAmountWidth ? _id % m_TileAmountWidth : m_TileAmountWidth)-1)+ margin;
		_id++;
		m_TilesPos[gid] = new Vec2i(x, y);
		if (m_ImgSource == "assets/example/maps/flowers.png") {
			LogConsole(MESSAGE_TYPE::WARNING, "TILE POS X= %d // POS Y=%d", x, y);
		}
	}
}

void Tilesets::ProcessTile(const Tmx::Tile* p_Tile, int gid)
{
	/*if (p_Tile->IsAnimated())
	{
		printf(
			"Tile is animated: %d frames with total duration of %dms.\n",
			p_Tile->GetFrameCount(), p_Tile->GetTotalDuration());

		const std::vector<Tmx::AnimationFrame> &frames =
			p_Tile->GetFrames();

		int i = 0;
		for (std::vector<Tmx::AnimationFrame>::const_iterator it =
			frames.begin(); it != frames.end(); it++, i++)
		{
			printf("\tFrame %d: Tile ID = %d, Duration = %dms\n", i,
				it->GetTileID(), it->GetDuration());
		}
		//TREAT ANIMATIONS!
	}*/

	if (p_Tile->HasObjects()) ///PROCESS COLLISION!
	{
		if (p_Tile->GetType() != "")
			printf("Tile has type: %s\n", p_Tile->GetType().c_str());

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
					auto tile_Object = new TilesetObjectData{ Vec2f((float)object->GetX(), (float)object->GetY()), polyBufferPoints, Physics2D::BodyShape::POLYGON, Physics2D::BodyType::STATIC };
					m_TileObjects[gid].push_back(tile_Object);
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
					auto tile_Object = new TilesetObjectData{ Vec2f((float)object->GetX(), (float)object->GetY()), polyBufferPoints, Physics2D::BodyShape::POLYGON, Physics2D::BodyType::STATIC };
					m_TileObjects[gid].push_back(tile_Object);
				}
			}else {
				/*auto body2 = Physics2D::PhysicsBody::CreateBody
				(
					Physics2D::PhysicsEngine::GetPhysicsWorld(), Physics2D::BodyType::STATIC,
					Physics2D::BodyShape::BOX, Physics2D::BodyParams{ 1.f,0.1f },
					Vec2f{ (float)object->GetX() + object->GetWidth() / PTM, (float)object->GetY() + object->GetHeight() / PTM },
					std::vector<Vec2f>{Vec2f(object->GetWidth() / PTM, object->GetHeight() / PTM)}
				);*/
				auto vecPos = Vec2f((float)object->GetX() + object->GetWidth() / PTM, (float)object->GetY() + object->GetHeight() / PTM);
				auto vecCoord = std::vector<Vec2f>{ Vec2f(object->GetWidth() / PTM, object->GetHeight() / PTM) };
				auto tile_Object = new TilesetObjectData{ vecPos, vecCoord, Physics2D::BodyShape::BOX, Physics2D::BodyType::STATIC };
				m_TileObjects[gid].push_back(tile_Object);
			}
		}
	}
}