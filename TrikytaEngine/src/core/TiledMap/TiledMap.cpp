#include "TiledMap.h"
#include "Tilesets.h"
#include "core/Common/TrikytaEngine.h"
#include "Parser/Tmx.h"
#include "core/Common/Vec2.h"
#include "core/Common/defines.h"
#include "ObjectGroup.h"
#include <SDL/SDL.h>
#include "core/Physics/PhysicsEngine.h"

int PrintMapInfo(Tmx::Map* map);//delete this later!

TiledMap* TiledMap::Create(std::string p_Filename) 
{
	Tmx::Map* map = new Tmx::Map();
	map->ParseFile(p_Filename);
	if (map->HasError())
	{
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
	for (auto& itr : *m_LayerData) {
		FREE(itr.tiledLayerData);
	}
	FREE(m_LayerData);
	FREE(m_MapTilesets);
	FREE(m_Map);
}

bool TiledMap::init() 
{
	isReady = false;
	m_Position = Vec2i(0,0);
	TiledMap::LoadTilesets();
	//auto LoadThread = new std::thread(&TiledMap::LoadLayers, this);
	auto gRenderer = ENGINE->getRenderer();
	m_MapDst = { 0, 0, m_Map->GetWidth()*m_Map->GetTileWidth(), m_Map->GetHeight()*m_Map->GetTileHeight() };
	m_MapSrc = { m_Position.x, m_Position.y, ENGINE->GetScreenWidth(), ENGINE->GetScreenHeight()}; //TODO: fix these hardcoded values!
	m_MapTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_Map->GetWidth()*m_Map->GetTileWidth(), m_Map->GetHeight()*m_Map->GetTileHeight());
	TiledMap::LoadLayers();
	return true;
}

void TiledMap::render(float dt) 
{
	if (!isReady) { return; }
	auto r = ENGINE->getRenderer();
	SDL_SetRenderTarget(r, m_MapTexture);
	for (auto& itr : *m_LayerData) {
		if (itr.tiledLayerData->isAnimated) {
			itr.tiledLayerData->LastDeltaTime = itr.tiledLayerData->LastDeltaTime + dt; 
			if (itr.tiledLayerData->LastDeltaTime > 0.1f) {
				itr.tiledLayerData->m_CurrentFrame++;
				if (itr.tiledLayerData->m_CurrentFrame >= itr.tiledLayerData->m_FramesVec.size()) {
					itr.tiledLayerData->m_CurrentFrame = 0;
				}
				itr.tiledLayerData->LastDeltaTime = 0;
			}
			SDL_RenderCopy(r, itr.tiledLayerData->m_FramesVec.at(itr.tiledLayerData->m_CurrentFrame)->Tex, 
				itr.tiledLayerData->m_FramesVec.at(itr.tiledLayerData->m_CurrentFrame)->SourceDraw, itr.tiledLayerData->DestDraw);
		}else{
			SDL_RenderCopy(r, itr.tiledLayerData->Tex, itr.tiledLayerData->SourceDraw, itr.tiledLayerData->DestDraw);
		}
	}
	SDL_SetRenderTarget(r, NULL);
	SDL_SetTextureBlendMode(m_MapTexture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(r, m_MapTexture, &m_MapSrc, NULL);
	//LogTerminal("Draw coord (x=%d, y=%d, w=%d, h=%d) // Dest Coord (%d, %d)", m_MapSrc.x, m_MapSrc.y, m_MapSrc.w, m_MapSrc.h, m_MapDst.w, m_MapDst.h);
}

bool TiledMap::LoadTilesets()
{
	// Iterate through the tilesets.
	m_MapTilesets = new std::vector<Tilesets>;
	m_MapTilesets->reserve(m_Map->GetNumTilesets());
	for (int i = 0; i < m_Map->GetNumTilesets(); ++i)
	{
		m_MapTilesets->emplace_back(this, i);
		//Log("[TILEDMAPS] Tilesets ID: %d || Tileset name : %s", i, m_Map->GetTileset(i)->GetName().c_str())
	}
	return true;
}

void TiledMap::LoadLayers()
{
	int ind = 0;
	m_LayerData = new std::vector<LayerData>; 
	//std::map<int, std::vector<std::pair<int, unsigned int>>*> cachedLayerData;
	//std::map<int, TileData*> mapLayerData;
	for (int LayerIndex = 0; LayerIndex < m_Map->GetNumTileLayers(); ++LayerIndex) {
		const Tmx::TileLayer* tileLayer = m_Map->GetTileLayer(LayerIndex);
		Log("[TILED MAPS] Layer: %s (W: %d, H: %d)", tileLayer->GetName().c_str(), tileLayer->GetWidth(), tileLayer->GetHeight());
		for (int y = 0; y < tileLayer->GetHeight(); ++y) {
			for (int x = 0; x < tileLayer->GetWidth(); ++x) {
				if (tileLayer->GetTileTilesetIndex(x, y) != -1) {
					m_LayerData->reserve(m_LayerData->size() + 1);
					ind++;
					int TilesetIndex = tileLayer->GetTileTilesetIndex(x, y);
					Uint32 Gid = tileLayer->GetTileGid(x, y);
					auto TilesetData = &(m_MapTilesets->at(TilesetIndex));
					TileData* tileData = new TileData(*TilesetData->m_Tiles[Gid]);
					tileData->setPosition(Vec2i(x, y), this);
					tileData->IsPhy = false;
					if (TilesetData->m_TileObjects[Gid].size() > 0) {
						tileData->PhyBodys = new std::vector<Physics2D::PhysicsBody*>;
						tileData->PhyBodys->reserve(TilesetData->m_TileObjects[Gid].size());
						tileData->IsPhy = true;
						for (auto& itr : TilesetData->m_TileObjects[Gid]) {
							auto tileBody = Physics2D::PhysicsBody::CreateBody
							(
								Physics2D::PhysicsEngine::GetPhysicsWorld(), itr->m_BodyType,
								itr->m_BodyShape, Physics2D::BodyParams{ 1.f,0.1f },
								itr->m_ObjectPos + Vec2f((float)tileData->DestDraw->x, (float)tileData->DestDraw->y),
								itr->m_ObjectCoord
							);
							tileData->PhyBodys->push_back(tileBody);
							tileData->bodyOffsetPos = itr->m_ObjectPos;
						}
						m_allMapBodies.insert(m_allMapBodies.end(), tileData->PhyBodys->begin(), tileData->PhyBodys->end());
					}
					m_LayerData->emplace_back(LayerIndex, tileData);
				}
			}
		}
	}
	if (m_Group.getBodies().size() > 0) {
		if (m_allMapBodies.size() > 0) {
			m_allMapBodies.insert(m_allMapBodies.end(), m_Group.getBodies().begin(), m_Group.getBodies().end());
		}else {
			m_allMapBodies = m_Group.getBodies();
		}
	}
	isReady = true;
	Log("[TILEDMAPS] Gonna be %d Calls in render!", ind)
}

void TiledMap::setPosition(Vec2i pos)
{
	static Vec2i LastPositionTranslated = Vec2i(0, 0);
	if (Utility::IsInBox(Vec2i(pos.x, pos.y), Vec2i(0,0), Vec2i(m_MapDst.w, m_MapDst.h))) {
		m_Position = pos;
		m_MapSrc.x = pos.x;
		m_MapSrc.y = pos.y;
		for (auto& phyObj : m_allMapBodies) {
			Vec2f old_transform = phyObj->GetTransform().p;
			phyObj->SetTransform
			(
				Vec2f(old_transform.x-float(pos.x- LastPositionTranslated.x), old_transform.y-float(pos.y - LastPositionTranslated.y))
				,0.f
			);
		}
		LastPositionTranslated = pos;
	}
}

void TiledMap::translateMap(Vec2i pos)
{
	if (Utility::IsInBox(Vec2i(m_MapSrc.x - pos.x, m_MapSrc.y - pos.y), Vec2i(0, 0), Vec2i(m_MapDst.w, m_MapDst.h))) {
		m_Position += pos;
		m_MapSrc.x -= pos.x;
		m_MapSrc.y -= pos.y;
		for (auto& phyObj : m_allMapBodies) {
			//auto current_rect = itr.tiledLayerData->DestDraw;
			//itr.tiledLayerData->DestDraw->x = current_rect->x+pos.x;
			//itr.tiledLayerData->DestDraw->y = current_rect->y+pos.y;
			Vec2f old_transform = phyObj->GetTransform().p;
			phyObj->SetTransform(old_transform + Vec2f((float)(pos.x), (float)(pos.y)), 0.f);
		}
	}
}

Vec2i TiledMap::getPosition()
{
	return m_Position;
}





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