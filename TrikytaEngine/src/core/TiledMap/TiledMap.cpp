#include "TiledMap.h"
#include "Tilesets.h"
#include "core/Common/TrikytaEngine.h"
#include "Parser/Tmx.h"
#include "core/Common/Vec2.h"
#include "core/Common/defines.h"
#include "ObjectGroup.h"
#include <SDL/SDL.h>
#include "core/Physics/PhysicsEngine.h"
#include "core/Camera/Camera.h"
#include <map>
#include "core/Drawable/Sprite.h"

//TODO: correct the debug drawing that it stuck at one thing!
bool CameraUpdate = false;

int PrintMapInfo(Tmx::Map* map);//delete this later!

LayerData::~LayerData() { FREE(tiledLayerData); }

TiledMap* TiledMap::Create(std::string p_Filename) 
{
	Tmx::Map* map = new Tmx::Map();
	map->ParseFile(p_Filename);
	if (map->HasError()){
		Log("ERROR: Loading Tiled Map failed with code: %d\n", map->GetErrorCode());
		Log("ERROR: Loading Tiled Map failed with code text: %s\n", map->GetErrorText().c_str());
		delete map; // delete and return null ptr!
		return nullptr;
	}
	//PrintMapInfo(map);
	// Find assets path:
	auto const pos = p_Filename.find_last_of('/');
	p_Filename = p_Filename.substr(0, pos+1);
	return new TiledMap(map, p_Filename);
}

TiledMap::TiledMap(Tmx::Map* p_Map,std::string& p_AssetsPath) : m_Map(p_Map), m_AssetsPath(p_AssetsPath), m_Group(m_Map)
{
	TiledMap::init();
}

TiledMap::~TiledMap()
{
	for (auto itr : m_LayerData) {
		FREE(itr->tiledLayerData);
	}
	FREE(m_MapTilesets);
	FREE(m_Map);
}

bool TiledMap::init() 
{
	m_Position = Vec2i(0,0);
	m_Size = Vec2i(m_Map->GetWidth()*m_Map->GetTileWidth(), m_Map->GetHeight()*m_Map->GetTileHeight());
	Vec2i MaxRenderTextureSize = Vec2i(ENGINE->getRenderInfo().max_texture_width, ENGINE->getRenderInfo().max_texture_height);
	Vec2i maxGridSize = Vec2i(MaxRenderTextureSize.x - (MaxRenderTextureSize.x % m_Map->GetTileWidth()), MaxRenderTextureSize.y - (MaxRenderTextureSize.y % m_Map->GetTileHeight()));
	if (CameraUpdate) {// working but have to draw on other textures too
		m_DestinationDrawCoord = { m_Position.x, m_Position.y, ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight() };
		m_SourceDrawCoord = { 0, 0, ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight() }; //TODO: Change this to camera size
	}else{ // shows nothing
		m_DestinationDrawCoord = { m_Position.x, m_Position.y, m_Size.x, m_Size.y };
		m_SourceDrawCoord = { 0, 0, m_Size.x, m_Size.y }; //TODO: Change this to camera size
	}

	if (m_Size.x > maxGridSize.x || m_Size.y > maxGridSize.y) {
		LogTerminal("Performing corpping on the map size = (W: %d, H: %d) maximum size (W:%d, H:%d)", m_Size.x, m_Size.y, ENGINE->getRenderInfo().max_texture_width, ENGINE->getRenderInfo().max_texture_height);
		TiledMap::DivideMap(maxGridSize);//MAP DIVISION ALGORITHM 
		m_DestinationDrawCoord = { m_Position.x, m_Position.y, maxGridSize.x, maxGridSize.y };
		m_SourceDrawCoord = { 0, 0, maxGridSize.x, maxGridSize.y }; //TODO: Change this to camera size
	}
	//TODO FIX TEXTURE SIZE!
	if (m_MapGrids.size() > 0) {
		m_Texture = m_MapGrids.at(0).m_Texture;
	} else {
		m_Texture = SDL_CreateTexture(ENGINE->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_Size.x, m_Size.y);
	}
	TiledMap::LoadTilesets();
	TiledMap::LoadLayers();
	return true;
}

void TiledMap::DivideMap(Vec2i& maxGridSize)
{
	Vec2i map_size = Vec2i(m_Size);
	Vec2i accumlator = Vec2i(0, 0);
	Vec2i texture_size = Vec2i(0, 0);
	Vec2i pos = Vec2i(0, 0);
	auto gRenderer = ENGINE->getRenderer();
	while ((accumlator.x < m_Size.x || accumlator.y < m_Size.y)) {
		// Size overflow we gonna use an algorithm to cut the map into grids! (on old GPUs)
		if (map_size.x <= 0) { // if it last one to cut then move to y
			accumlator.y += texture_size.y;
			pos.y += texture_size.y;
			pos.x = 0;
			if (accumlator.y >= m_Size.y) {
				break;
			}
			map_size.x = m_Size.x; // reset to begining !
			accumlator.x = m_Size.x;
			map_size.y = map_size.y - maxGridSize.y; // update the y size
		}
		texture_size.x = (map_size.x - maxGridSize.x > 0) ? maxGridSize.x : map_size.x;
		texture_size.y = (map_size.y - maxGridSize.y > 0) ? maxGridSize.y : map_size.y;
		map_size.x -= texture_size.x; // update x
		auto map_grid_texture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_size.x, texture_size.y);
		m_MapGrids.emplace_back(map_grid_texture, SDL_Rect{pos.x, pos.y, texture_size.x, texture_size.y}, Vec2i(pos.x, pos.y));
		///LogTerminal("Pos division (%d, %d)", m_MapGrids.back().m_Coords.x, m_MapGrids.back().m_Coords.y);
		pos.x += texture_size.x;
	}
}

void TiledMap::renderAnimations(float dt)
{
	auto r = ENGINE->getRenderer();
	for (auto& itr : m_cachedImmediateTiles) {
		if (itr->tiledLayerData->isAnimated) {
			itr->tiledLayerData->LastDeltaTime = itr->tiledLayerData->LastDeltaTime + dt;
			if (itr->tiledLayerData->LastDeltaTime > itr->tiledLayerData->m_FramesVec.at(itr->tiledLayerData->m_CurrentFrame)->m_AnimationDuration / 1000.f) {
				itr->tiledLayerData->m_CurrentFrame++;
				if (itr->tiledLayerData->m_CurrentFrame >= itr->tiledLayerData->m_FramesVec.size()) {
					itr->tiledLayerData->m_CurrentFrame = 0;
				}
				itr->tiledLayerData->LastDeltaTime = 0;
			}
			SDL_RenderCopy(r, itr->tiledLayerData->m_FramesVec.at(itr->tiledLayerData->m_CurrentFrame)->Tex,
				itr->tiledLayerData->m_FramesVec.at(itr->tiledLayerData->m_CurrentFrame)->SourceDraw, itr->tiledLayerData->DestDraw);
		}else{
			SDL_RenderCopy(r, itr->tiledLayerData->Tex,
				itr->tiledLayerData->SourceDraw, itr->tiledLayerData->DestDraw);
		}
	}
}

void TiledMap::render(float dt)
{
	auto r = ENGINE->getRenderer();
	renderAnimations(dt);
	if (m_MapGrids.size() > 0) {
		for (MapPart& texture_data : m_MapGrids) {
			SDL_SetTextureBlendMode(texture_data.m_Texture, SDL_BLENDMODE_BLEND);
			SDL_RenderCopyEx(r, texture_data.m_Texture, NULL, &texture_data.m_Coords, m_Angle, &m_RotationCenter, m_Flip);
			///Draw the debug corrping!
			//SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 200);
			//SDL_RenderDrawRect(ENGINE->getRenderer(), &texture_data.m_Coords);
		}
	}else{
		SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
		if (CameraUpdate) {
			SDL_RenderCopyEx(r, m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
		}else{
			SDL_RenderCopyEx(r, m_Texture, &m_SourceDrawCoord, &m_DestinationDrawCoord, m_Angle, &m_RotationCenter, m_Flip);
		}
	}
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);
}

bool TiledMap::LoadTilesets()
{
	// Iterate through the tilesets.
	m_MapTilesets = new std::vector<Tilesets>;
	m_MapTilesets->reserve(m_Map->GetNumTilesets());
	for (int i = 0; i < m_Map->GetNumTilesets(); ++i){
		m_MapTilesets->emplace_back(this, i);
		//m_MapTilesetsByName[m_Map->GetTileset(i)->GetName()] = &m_MapTilesets->back();
	}
	return true;
}

void TiledMap::LoadLayers()
{
	for (int LayerIndex = 0; LayerIndex < m_Map->GetNumTileLayers(); ++LayerIndex) {
		const Tmx::TileLayer* tileLayer = m_Map->GetTileLayer(LayerIndex);
		Log("[TILED MAPS] Layer: %s (W: %d, H: %d)", tileLayer->GetName().c_str(), tileLayer->GetWidth(), tileLayer->GetHeight());
		for (int y = 0; y < tileLayer->GetHeight(); ++y) {
			for (int x = 0; x < tileLayer->GetWidth(); ++x) {
				if (tileLayer->GetTileTilesetIndex(x, y) != -1) {
					m_LayerData.reserve(m_LayerData.size() + 1);
					int TilesetIndex = tileLayer->GetTileTilesetIndex(x, y);
					Uint32 Gid = tileLayer->GetTileGid(x, y);
					auto TilesetData = &(m_MapTilesets->at(TilesetIndex));
					TileData* tileData = new TileData(*TilesetData->m_Tiles[Gid]); // should we copy this right here ?
					tileData->setPosition(Vec2i(x, y), this);
					tileData->m_MapGrid = Vec2i(x, y);
					tileData->IsPhy = false;
					m_LayerData.emplace_back(new LayerData(LayerIndex, tileData));
					if ((tileLayer->GetProperties().HasProperty("immediate") && tileLayer->GetProperties().GetBoolProperty("immediate")) || tileData->isAnimated) {
						tileData->m_LayerType = LayerType::IMMEDIATE;
						m_ImmediateLayerData[std::make_pair(tileData->TileName, x*y)] = m_LayerData.back();
					}else {
						tileData->m_LayerType = LayerType::RETAINED;
					}
					if (TilesetData->m_TileObjects[Gid].size() > 0) {
						tileData->IsPhy = true;
						for (auto bp : TilesetData->m_TileObjects[Gid]) {
							auto body = Physics2D::PhysicsBody::CreateBody(
								Physics2D::PhysicsEngine::GetPhysicsWorld(),bp.m_BodyType, bp.m_BodyShape, bp.m_BodyParams, 
								bp.m_ObjectPos + Vec2f(float(tileData->DestDraw->x), float(tileData->DestDraw->y)), bp.m_ObjectCoord
							);
							tileData->PhyBodys.emplace_back(body);
							m_BodyByTile[tileData->TileName][tileData->id-1].emplace_back(body);
							//m_BodyByTile[std::make_pair(tileData->TileName, tileData->id - 1)].emplace_back(body);//switch to this after
							m_allMapBodies.emplace_back(body);
							body->addComponent(m_LayerData.back());
						}
					}
				}
			}
		}
	}
	if (m_Group.getBodies().size() > 0) {
		if (m_allMapBodies.size() > 0) {
			m_allMapBodies.reserve(m_allMapBodies.size() + m_Group.getBodies().size());
			auto tempVec = m_Group.getBodies();
			m_allMapBodies.insert(m_allMapBodies.end(), tempVec.begin(), tempVec.end());
		}else {
			m_allMapBodies = m_Group.getBodies();
		}
	}
	LoadMapIntoTexture();
	Log("[TILEDMAPS] Map loaded sucesfully and ready to be rendered !");
}

void TiledMap::LoadMapIntoTexture()
{
	auto r = ENGINE->getRenderer();
	if (m_MapGrids.size() > 0) {
		for (auto& texture_data : m_MapGrids) { //TODO: fix this to only draw when its inside
			SDL_SetRenderTarget(r, texture_data.m_Texture);
			Vec2i min = Vec2i(texture_data.m_Coords.x, texture_data.m_Coords.y);
			Vec2i max = Vec2i(texture_data.m_Coords.x, texture_data.m_Coords.y) + Vec2i(texture_data.m_Coords.w, texture_data.m_Coords.h);
			for (auto& itr : m_LayerData) {
				if (itr->tiledLayerData->m_LayerType == LayerType::IMMEDIATE) {
					m_cachedImmediateTiles.emplace_back(itr);
				}else{
					//Perform corpping!
					if (Utility::IsInBox(Vec2i(itr->tiledLayerData->DestDraw->x, itr->tiledLayerData->DestDraw->y), min, max)){
						SDL_Rect tempDestDraw;
						memcpy(&tempDestDraw, itr->tiledLayerData->DestDraw, sizeof(SDL_Rect));
						tempDestDraw.x -= min.x;
						tempDestDraw.y -= min.y;
						SDL_RenderCopy(r, itr->tiledLayerData->Tex, itr->tiledLayerData->SourceDraw, &tempDestDraw);
					}
				}
			}
		}
	}else{
		SDL_SetRenderTarget(r, m_Texture);
		for (auto& itr : m_LayerData) {
			if (itr->tiledLayerData->m_LayerType == LayerType::IMMEDIATE) {
				m_cachedImmediateTiles.emplace_back(itr);
			}
			else {
				SDL_RenderCopy(r, itr->tiledLayerData->Tex, itr->tiledLayerData->SourceDraw, itr->tiledLayerData->DestDraw);
			}
		}
	}
	//AddPhysicsDebugDrawToMapTexture();
	SDL_SetRenderTarget(r, NULL);
}

void TiledMap::setPosition(Vec2i pos)
{
	static Vec2i LastPositionTranslated = Vec2i(0, 0);
	if (CameraUpdate) {
		if (pos.x >= 0 && pos.x <= m_Size.x-m_DestinationDrawCoord.w) {
			m_SourceDrawCoord.x = pos.x; // remove negative sign for the real source here source is used as Dest!
		}
		if (pos.y >= 0 && pos.y <= m_Size.y- m_DestinationDrawCoord.h) {
			m_SourceDrawCoord.y = pos.y; // remove negative sign for the real source here source is used as Dest!
		}
		for (auto& itr : m_cachedImmediateTiles) {
			auto current_rect = itr->tiledLayerData->DestDraw;
			itr->tiledLayerData->DestDraw->x = current_rect->x - (pos.x - LastPositionTranslated.x);
			itr->tiledLayerData->DestDraw->y = current_rect->y - (pos.y - LastPositionTranslated.y);
		}
		/*for (auto& phyObj : m_allMapBodies) {
			Vec2f old_transform = phyObj->GetTransform().p;
			phyObj->SetTransform
			(
				Vec2f(old_transform.x-float(pos.x - LastPositionTranslated.x), old_transform.y-float(pos.y - LastPositionTranslated.y))
				,0.f
			);
		}*/
	}else{
		if (m_MapGrids.size() > 0) {
			//LogTerminal("-------------------------------------------------------");
			for (auto& texture_data : m_MapGrids) {
				texture_data.m_Coords.x = texture_data.m_InitPos.x - pos.x;
				texture_data.m_Coords.y = texture_data.m_InitPos.y - pos.y;
				//LogTerminal("x=%d, y=%d", texture_data.m_Coords.x, texture_data.m_Coords.y);
			}
			//LogTerminal("-------------------------------------------------------");
		} else {
			m_DestinationDrawCoord.x = -pos.x; // remove negative sign for the real source here source is used as Dest!
			m_DestinationDrawCoord.y = -pos.y;
		}
		for (auto& itr : m_cachedImmediateTiles) {
			auto current_rect = itr->tiledLayerData->DestDraw;
			itr->tiledLayerData->DestDraw->x = current_rect->x - (pos.x - LastPositionTranslated.x);
			itr->tiledLayerData->DestDraw->y = current_rect->y - (pos.y - LastPositionTranslated.y);
		}
	}
	LastPositionTranslated = pos;
}

void TiledMap::translateMap(Vec2i pos)
{
	if (Utility::IsInBox(Vec2i(m_SourceDrawCoord.x - pos.x, m_SourceDrawCoord.y - pos.y), Vec2i(0, 0), Vec2i(m_DestinationDrawCoord.w, m_DestinationDrawCoord.h))) {
		m_Position += pos;
		m_SourceDrawCoord.x -= pos.x;
		m_SourceDrawCoord.y -= pos.y;
		for (auto& itr : m_cachedImmediateTiles) {
			auto current_rect = itr->tiledLayerData->DestDraw;
			itr->tiledLayerData->DestDraw->x = current_rect->x + pos.x;
			itr->tiledLayerData->DestDraw->y = current_rect->y + pos.y;
		}
		/*for (auto& phyObj : m_allMapBodies) {
			Vec2f old_transform = phyObj->GetTransform().p;
			phyObj->SetTransform(old_transform + Vec2f((float)(pos.x), (float)(pos.y)), 0.f);
		}*/
	}
}

Vec2i TiledMap::getPosition()
{
	return m_Position;
}

void TiledMap::AddPhysicsDebugDrawToMapTexture()
{
	auto r = ENGINE->getRenderer();
	for (auto& phyObj : m_allMapBodies) {
		const b2Transform& xf = phyObj->GetTransform();
		for (b2Fixture* f = phyObj->GetFixtureList(); f; f = f->GetNext())
		{
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];

			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}
			for (int i = 0; i < vertexCount - 1; i++)
			{
				auto vec1 = b2Vec2(vertices[i].x, vertices[i].y);
				auto vec2 = b2Vec2(vertices[i + 1].x, vertices[i + 1].y);
				lineRGBA(ENGINE->getRenderer(), (Sint16)vec1.x, (Sint16)vec1.y, (Sint16)vec2.x, (Sint16)vec2.y, 0, 0, 0, 125);
			}
			int i = vertexCount - 1;
			auto vec1 = b2Vec2(vertices[i].x, vertices[i].y);
			auto vec2 = b2Vec2(vertices[0].x, vertices[0].y);
			lineRGBA(ENGINE->getRenderer(), (Sint16)vec1.x, (Sint16)vec1.y, (Sint16)vec2.x, (Sint16)vec2.y, 0, 0, 0, 125);
			SDL_SetRenderDrawColor(r, 0x00, 0x00, 0x00, 0xFF);
		}
	}
}

void TiledMap::setAffectedByCamera(Camera* cam) 
{ 
	Drawable::setAffectedByCamera(cam);
	if (!isAffectedByCamera()) { return; }
	for (const auto& objectsThatCanBeAffectedByCam : m_Group.getDrawables()) {
		m_Camera->addObjectToCamera(objectsThatCanBeAffectedByCam);
	}
}

const std::vector<Physics2D::PhysicsBody*>& TiledMap::getTilesetBodiesByID(const std::string& tilsetName, int id)
{
	return m_BodyByTile[tilsetName][id];
}

bool TiledMap::isBodyPartOfTileset(Physics2D::PhysicsBody* body, const std::string& tilsetName, int id)
{
	if (!m_BodyByTile[tilsetName][id].empty()) {
		return std::find(m_BodyByTile[tilsetName][id].begin(), m_BodyByTile[tilsetName][id].end(), body) != m_BodyByTile[tilsetName][id].end();
	}else{
		return false;
	}
}

void TiledMap::DeleteTileInLayer(LayerData* tileToDelete)
{
	auto itr = std::find(m_cachedImmediateTiles.begin(), m_cachedImmediateTiles.end(), tileToDelete);
	if (itr != m_cachedImmediateTiles.end()) {
		m_cachedImmediateTiles.erase(itr);
		FREE(tileToDelete);
	}
}

/*const std::vector<Physics2D::PhysicsBody*>& TiledMap::getTilesetBodiesByID(const std::string& tilsetName, int id)
{
	return m_BodyByTile[std::make_pair(tilsetName, id)];
}

bool TiledMap::isBodyPartOfTileset(Physics2D::PhysicsBody* body, const std::string& tilsetName, int id)
{
	if (m_BodyByTile.find(std::make_pair(tilsetName, id)) != m_BodyByTile.end()) {
		return std::find(m_BodyByTile[std::make_pair(tilsetName, id)].begin(),
			m_BodyByTile[std::make_pair(tilsetName, id)].end(), body) != m_BodyByTile[std::make_pair(tilsetName, id)].end();
	}
	else {
		return false;
	}
}*/

/////////////////////////////////////////////////
int PrintMapInfo(Tmx::Map* map)
{
	if (map->HasError())
	{
		printf("error code: %d\n", map->GetErrorCode());
		printf("error text: %s\n", map->GetErrorText().c_str());

		return map->GetErrorCode();
	}

	printf("====================================\n");
	printf("Map\n");
	printf("====================================\n");

	printf("Version: %1.1f\n", map->GetVersion());
	printf("Orientation: %d\n", map->GetOrientation());
	if (!map->GetBackgroundColor().IsTransparent())
		printf("Background Color (hex): %s\n",
			map->GetBackgroundColor().ToString().c_str());
	printf("Render Order: %d\n", map->GetRenderOrder());
	if (map->GetStaggerAxis())
		printf("Stagger Axis: %d\n", map->GetStaggerAxis());
	if (map->GetStaggerIndex())
		printf("Stagger Index: %d\n", map->GetStaggerIndex());
	printf("Width: %d\n", map->GetWidth());
	printf("Height: %d\n", map->GetHeight());
	printf("Tile Width: %d\n", map->GetTileWidth());
	printf("Tile Height: %d\n", map->GetTileHeight());

	// Iterate through map properties and print the type, name and value of each property.
	const std::unordered_map<std::string, Tmx::Property> &mapProperties = map->GetProperties().GetPropertyMap();
	for (auto &pair : mapProperties)
	{
		const Tmx::Property &property = pair.second;

		std::string type;

		if (property.GetType() == Tmx::TMX_PROPERTY_STRING)
		{
			type = "String";
		}
		else if (property.GetType() == Tmx::TMX_PROPERTY_FLOAT)
		{
			type = "Float";
		}
		else if (property.GetType() == Tmx::TMX_PROPERTY_INT)
		{
			type = "Integer";
		}
		else if (property.GetType() == Tmx::TMX_PROPERTY_BOOL)
		{
			type = "Boolean";
		}
		else if (property.GetType() == Tmx::TMX_PROPERTY_COLOR)
		{
			type = "Color";
		}
		else if (property.GetType() == Tmx::TMX_PROPERTY_FILE)
		{
			type = "File";
		}
		else
		{
			type = "Unknown";
		}

		printf("Map property %s (%s) = %s\n", pair.first.c_str(), type.c_str(), property.GetValue().c_str());
	}

	// Iterate through the tilesets.
	for (int i = 0; i < map->GetNumTilesets(); ++i)
	{
		printf("                                    \n");
		printf("====================================\n");
		printf("Tileset : %02d\n", i);
		printf("====================================\n");

		// Get a tileset.
		const Tmx::Tileset *tileset = map->GetTileset(i);

		// Print tileset information.
		printf("Name: %s\n", tileset->GetName().c_str());
		printf("Margin: %d\n", tileset->GetMargin());
		printf("Spacing: %d\n", tileset->GetSpacing());
		printf("First gid: %d\n", tileset->GetFirstGid());
		printf("Image Width: %d\n", tileset->GetImage()->GetWidth());
		printf("Image Height: %d\n", tileset->GetImage()->GetHeight());
		printf("Image Source: %s\n", tileset->GetImage()->GetSource().c_str());
		if (!tileset->GetImage()->GetTransparentColor().IsTransparent())
			printf("Transparent Color (hex): %s\n",
				tileset->GetImage()->GetTransparentColor().ToString().c_str());

		if (tileset->GetTiles().size() > 0)
		{
			// Get a tile from the tileset.
			const Tmx::Tile *tile = *(tileset->GetTiles().begin());

			// Print the properties of a tile.
			std::map<std::string, std::string> list =
				tile->GetProperties().GetList();
			std::map<std::string, std::string>::iterator iter;
			for (iter = list.begin(); iter != list.end(); ++iter)
			{
				printf("%s = %s\n", iter->first.c_str(), iter->second.c_str());
			}

			if (tile->IsAnimated())
			{
				printf(
					"Tile is animated: %d frames with total duration of %dms.\n",
					tile->GetFrameCount(), tile->GetTotalDuration());

				const std::vector<Tmx::AnimationFrame> &frames =
					tile->GetFrames();

				int i = 0;
				for (std::vector<Tmx::AnimationFrame>::const_iterator it =
					frames.begin(); it != frames.end(); it++, i++)
				{
					printf("\tFrame %d: Tile ID = %d, Duration = %dms\n", i,
						it->GetTileID(), it->GetDuration());
				}
			}

			if (tile->HasObjects())
			{
				printf(
					"Tile has objects.\n");

				if (tile->GetType() != "")
					printf("Tile has type: %s\n", tile->GetType().c_str());

				// Iterate through all Collision objects in the tile.
				for (int j = 0; j < tile->GetNumObjects(); ++j)
				{
					// Get an object.
					const Tmx::Object *object = tile->GetObject(j);

					// Print information about the object.
					printf("Object Name: %s\n", object->GetName().c_str());
					printf("Object Position: (%03d, %03d)\n", object->GetX(),
						object->GetY());
					printf("Object Size: (%03d, %03d)\n", object->GetWidth(),
						object->GetHeight());

					// Print Polygon points.
					const Tmx::Polygon *polygon = object->GetPolygon();
					if (polygon != 0)
					{
						for (int i = 0; i < polygon->GetNumPoints(); i++)
						{
							const Tmx::Point &point = polygon->GetPoint(i);
							printf("Object Polygon: Point %d: (%f, %f)\n", i, point.x,
								point.y);
						}
					}

					// Print Polyline points.
					const Tmx::Polyline *polyline = object->GetPolyline();
					if (polyline != 0)
					{
						for (int i = 0; i < polyline->GetNumPoints(); i++)
						{
							const Tmx::Point &point = polyline->GetPoint(i);
							printf("Object Polyline: Point %d: (%f, %f)\n", i, point.x,
								point.y);
						}
					}
				}

			}
		}
	}

	// Iterate through the tile layers.
	for (int i = 0; i < map->GetNumTileLayers(); ++i)
	{
		printf("                                    \n");
		printf("====================================\n");
		printf("Layer : %02d/%s \n", i, map->GetTileLayer(i)->GetName().c_str());
		printf("====================================\n");

		// Get a layer.
		const Tmx::TileLayer *tileLayer = map->GetTileLayer(i);

		for (int y = 0; y < tileLayer->GetHeight(); ++y)
		{
			for (int x = 0; x < tileLayer->GetWidth(); ++x)
			{
				if (tileLayer->GetTileTilesetIndex(x, y) == -1)
				{
					printf("........    ");
				}
				else
				{
					// Get the tile's id and gid.
					printf("%03d(%03d)", tileLayer->GetTileId(x, y), tileLayer->GetTileGid(x, y));

					// Find a tileset for that id.
					//const Tmx::Tileset *tileset = map->FindTileset(layer->GetTileId(x, y));
					if (tileLayer->IsTileFlippedHorizontally(x, y))
					{
						printf("h");
					}
					else
					{
						printf(" ");
					}
					if (tileLayer->IsTileFlippedVertically(x, y))
					{
						printf("v");
					}
					else
					{
						printf(" ");
					}
					if (tileLayer->IsTileFlippedDiagonally(x, y))
					{
						printf("d ");
					}
					else
					{
						printf("  ");
					}
				}
			}

			printf("\n");
		}
	}

	printf("\n\n");

	// Iterate through all of the object groups.
	for (int i = 0; i < map->GetNumObjectGroups(); ++i)
	{
		printf("                                    \n");
		printf("====================================\n");
		printf("Object group : %02d\n", i);
		printf("====================================\n");

		// Get an object group.
		const Tmx::ObjectGroup *objectGroup = map->GetObjectGroup(i);

		// Iterate through all objects in the object group.
		for (int j = 0; j < objectGroup->GetNumObjects(); ++j)
		{
			// Get an object.
			const Tmx::Object *object = objectGroup->GetObject(j);

			// Print information about the object.
			printf("Object Name: %s\n", object->GetName().c_str());
			printf("Object Position: (%03d, %03d)\n", object->GetX(),
				object->GetY());
			printf("Object Size: (%03d, %03d)\n", object->GetWidth(),
				object->GetHeight());

			if (object->GetGid() != 0) {
				printf("Object(tile) gid: %d\n", object->GetGid());
				printf("Object(tile) type: %s\n", object->GetType().c_str());
			}

			// Print Polygon points.
			const Tmx::Polygon *polygon = object->GetPolygon();
			if (polygon != 0)
			{
				for (int i = 0; i < polygon->GetNumPoints(); i++)
				{
					const Tmx::Point &point = polygon->GetPoint(i);
					printf("Object Polygon: Point %d: (%f, %f)\n", i, point.x,
						point.y);
				}
			}

			// Print Polyline points.
			const Tmx::Polyline *polyline = object->GetPolyline();
			if (polyline != 0)
			{
				for (int i = 0; i < polyline->GetNumPoints(); i++)
				{
					const Tmx::Point &point = polyline->GetPoint(i);
					printf("Object Polyline: Point %d: (%f, %f)\n", i, point.x,
						point.y);
				}
			}

			// Print Text information
			const Tmx::Text *text = object->GetText();
			if (text != 0)
			{
				printf("--Object Text--\n");
				printf("Font family: %s\n", text->GetFontFamily().c_str());
				printf("Pixel size: %d\n", text->GetPixelSize());
				printf("Wraps: %d\n", text->Wraps());
				printf("Bold: %d, Italic: %d, Underline: %d, Strikeout: %d\n", text->IsBold(), text->IsItalic(),
					text->IsUnderline(), text->IsStrikeout());
				printf("Kerning: %d\n", text->UseKerning());
				printf("Horizontal ALignment: %d\n", text->GetHorizontalAlignment());
				printf("Vertical Alignment: %d\n", text->GetVerticalAlignment());
				printf("Color: %d, %d, %d, %d", text->GetColor()->GetRed(), text->GetColor()->GetGreen(),
					text->GetColor()->GetBlue(), text->GetColor()->GetAlpha());
			}
		}
	}

	// Iterate through all of the group layers.
	for (int i = 0; i < map->GetNumGroupLayers(); ++i) {
		printf("                                    \n");
		printf("====================================\n");
		printf("Group Layer : %02d/%s \n", i, map->GetGroupLayer(i)->GetName().c_str());
		printf("====================================\n");
		// Get a group layer.
		const Tmx::GroupLayer *groupLayer = map->GetGroupLayer(i);

		printf("Offset X: %d", groupLayer->GetOffsetX());
		printf("Offset Y: %d", groupLayer->GetOffsetY());
		printf("Number of Children: %d", groupLayer->GetNumChildren());

		for (int j = 0; j < groupLayer->GetNumChildren(); j++) {
			const Tmx::Layer *childLayer = groupLayer->GetChild(j);
			printf("Child Layer Name: %s\n", childLayer->GetName().c_str());
			printf("Child Layer Type: %02d\n", childLayer->GetLayerType());

			if (childLayer->GetLayerType() == Tmx::TMX_LAYERTYPE_GROUP_LAYER) {
				const Tmx::GroupLayer *childGroupLayer = static_cast<const Tmx::GroupLayer*>(childLayer);
				printf("  Child group layer number of children: %d\n", childGroupLayer->GetNumChildren());
				printf("  Child group layer first child name: %s\n", childGroupLayer->GetChild(0)->GetName().c_str());
				printf("  Child group layer first child type: %d\n", childGroupLayer->GetChild(0)->GetLayerType());
			}
		}
	}
	//getchar();
	return 0;
}