#include "ObjectGroup.h"
#include "Parser/Tmx.h"
#include "core/Physics/PhysicsEngine.h"

ObjectGroup::ObjectGroup(Tmx::Map* p_Map)
{
	ObjectGroup::ParseGroups(p_Map);
}

void ObjectGroup::ParseGroups(Tmx::Map* p_Map)
{
	for (int i = 0; i < p_Map->GetNumObjectGroups(); ++i)
	{
		// Get an object group.
		const Tmx::ObjectGroup *objectGroup = p_Map->GetObjectGroup(i);
		// Iterate through all objects in the object group.
		if (objectGroup->GetProperties().GetBoolProperty("collision") == true) {
			for (int j = 0; j < objectGroup->GetNumObjects(); ++j) {
				// Get an object.
				const Tmx::Object *object = objectGroup->GetObject(j);
				ObjectGroup::ProcessPhysicalizedObject(object);
				/*if (object->GetProperties().GetBoolProperty("collision") == true) {
					//Apply physics!

				}*/
				/*if (object->GetGid() != 0) {
					printf("Object(tile) gid: %d\n", object->GetGid());
					printf("Object(tile) type: %s\n", object->GetType().c_str());
				}*/
				// Print Polyline points.
				/*const Tmx::Polyline *polyline = object->GetPolyline();
				if (polyline != 0)
				{
					for (int i = 0; i < polyline->GetNumPoints(); i++)
					{
						const Tmx::Point &point = polyline->GetPoint(i);
						printf("Object Polyline: Point %d: (%f, %f)\n", i, point.x,
							point.y);
					}
				}*/

				// Print Text information
				/*const Tmx::Text *text = object->GetText();
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
				}*/
			}
		}
	}
}

void ObjectGroup::ProcessPhysicalizedObject(const Tmx::Object* p_Object)
{
	///TODO READ PHY PROP FROM TILED MAP!
	if (p_Object->GetHeight() == 0 && p_Object->GetWidth() == 0) {
		///Process Polygones
		const Tmx::Polygon *polygon = p_Object->GetPolygon();
		if (polygon != 0)
		{
			std::vector<Vec2f> polyBufferPoints;
			polyBufferPoints.reserve(polygon->GetNumPoints()+1);
			for (int i = 0; i < polygon->GetNumPoints(); i++)
			{
				const Tmx::Point &point = polygon->GetPoint(i);
				polyBufferPoints.emplace_back(point.x, point.y);
			}
			polyBufferPoints.emplace_back(polygon->GetPoint(0).x, polygon->GetPoint(0).y);
			auto body2 = Physics2D::PhysicsBody::CreateBody
			(
				Physics2D::PhysicsEngine::GetPhysicsWorld(), Physics2D::BodyType::STATIC,
				Physics2D::BodyShape::POLYGON, Physics2D::BodyParams{ 1.f,0.1f },
				Vec2f{ (float)p_Object->GetX(), (float)p_Object->GetY() },
				polyBufferPoints
			);
		}
		/// Process Polylines!
		const Tmx::Polyline *polyline = p_Object->GetPolyline();
		if (polyline != 0)
		{
			std::vector<Vec2f> polyBufferPoints;
			polyBufferPoints.reserve(polyline->GetNumPoints()+1);
			for (int i = 0; i < polyline->GetNumPoints(); i++)
			{
				const Tmx::Point &point = polyline->GetPoint(i);
				polyBufferPoints.emplace_back(point.x, point.y);
			}
			polyBufferPoints.emplace_back(polyline->GetPoint(0).x, polyline->GetPoint(0).y);
			auto body2 = Physics2D::PhysicsBody::CreateBody
			(
				Physics2D::PhysicsEngine::GetPhysicsWorld(), Physics2D::BodyType::STATIC,
				Physics2D::BodyShape::POLYGON, Physics2D::BodyParams{ 1.f,0.1f },
				Vec2f{ (float)p_Object->GetX(), (float)p_Object->GetY() },
				polyBufferPoints
			);
		}
	}else {
		///PROCESS CIRCLES AND BOXES
		auto body2 = Physics2D::PhysicsBody::CreateBody
		(
			Physics2D::PhysicsEngine::GetPhysicsWorld(), Physics2D::BodyType::STATIC,
			Physics2D::BodyShape::BOX, Physics2D::BodyParams{ 1.f,0.1f },
			Vec2f{ (float)p_Object->GetX() + p_Object->GetWidth() / PTM, (float)p_Object->GetY() + p_Object->GetHeight() / PTM },
			std::vector<Vec2f>{Vec2f(p_Object->GetWidth() / PTM, p_Object->GetHeight() / PTM)}
		);
	}
}

ObjectGroup::~ObjectGroup()
{

}