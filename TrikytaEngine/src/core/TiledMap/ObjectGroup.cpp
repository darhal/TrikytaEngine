#include "ObjectGroup.h"
#include "Parser/Tmx.h"
#include "core/Physics/PhysicsEngine.h"
#include "UI/UIText.h"
#include "core/Drawable/Drawable.h"

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
				/*if (object->GetGid() != 0) {
					printf("Object(tile) gid: %d\n", object->GetGid());
					printf("Object(tile) type: %s\n", object->GetType().c_str());
				}*/

				// Print Text information
				const Tmx::Text *text = object->GetText();
				if (text != 0)
				{
					Color text_color;
					int style = Font::Style::NORMAL;
					if (text->GetColor() != nullptr) {
						text_color.r = text->GetColor()->GetRed();
						text_color.g = text->GetColor()->GetGreen();
						text_color.b = text->GetColor()->GetBlue();
						text_color.a = text->GetColor()->GetAlpha();
					}
					if (text->IsBold()){
						style |= Font::Style::BOLD;
					}
					if (text->IsItalic()) {
						style |= Font::Style::ITALIC;
					}
					if (text->IsUnderline()) {
						style |= Font::Style::UNDERLINE;
					}
					if (text->IsStrikeout()) {
						style |= Font::Style::STRIKETHROUGH;
					}
					auto ui_text = UI::Text::createText(object->GetText()->GetText(), "Engine_Assets/fonts/DroidSans.ttf", text->GetPixelSize(), Vec2i(object->GetX(), object->GetY()), text_color, style);
					m_Drawables.emplace_back(ui_text);
				}
			}
		}
	}
}

void ObjectGroup::ProcessPhysicalizedObject(const Tmx::Object* p_Object)
{
	Physics2D::BodyType b_type = Physics2D::BodyType::STATIC;
	Physics2D::BodyParams b_params;
	GetPhysicsSettings(p_Object, b_type, b_params);
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
			auto body = Physics2D::PhysicsBody::CreateBody
			(
				Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,
				Physics2D::BodyShape::POLYGON, b_params,
				Vec2f{ (float)p_Object->GetX(), (float)p_Object->GetY() },
				polyBufferPoints
			);
			m_Bodies.emplace_back(body);
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
			auto body = Physics2D::PhysicsBody::CreateBody
			(
				Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,
				Physics2D::BodyShape::POLYGON, b_params,
				Vec2f{ (float)p_Object->GetX(), (float)p_Object->GetY() },
				polyBufferPoints
			);
			m_Bodies.emplace_back(body);
		}
	}else {
		///PROCESS CIRCLES AND BOXES
		auto body = Physics2D::PhysicsBody::CreateBody
		(
			Physics2D::PhysicsEngine::GetPhysicsWorld(), b_type,
			Physics2D::BodyShape::BOX, b_params,
			Vec2f{ (float)p_Object->GetX() + p_Object->GetWidth() / PTM, (float)p_Object->GetY() + p_Object->GetHeight() / PTM },
			std::vector<Vec2f>{Vec2f(p_Object->GetWidth() / PTM, p_Object->GetHeight() / PTM)}
		);
		m_Bodies.emplace_back(body);
	}
}

void ObjectGroup::GetPhysicsSettings(const Tmx::Object* p_Object, Physics2D::BodyType& p_type, Physics2D::BodyParams& p_params)
{
	if (p_Object->GetProperties().HasProperty("type")) {
		if (p_Object->GetProperties().GetStringProperty("type") == "kinematic") {
			p_type = Physics2D::BodyType::KINEMATIC;
		}
		else if (p_Object->GetProperties().GetStringProperty("type") == "dynmaic") {
			p_type = Physics2D::BodyType::DYNAMIC;
		}
	}
	if (p_Object->GetProperties().HasProperty("mass")) {
		p_params.mass = p_Object->GetProperties().GetFloatProperty("mass");
	}
	if (p_Object->GetProperties().HasProperty("friction")) {
		p_params.friction = p_Object->GetProperties().GetFloatProperty("friction");
	}
	if (p_Object->GetProperties().HasProperty("restitution")) {
		p_params.restitution = p_Object->GetProperties().GetFloatProperty("restitution");
	}
	if (p_Object->GetProperties().HasProperty("sensor")) {
		p_params.sensor = p_Object->GetProperties().GetBoolProperty("sensor");
	}
}

ObjectGroup::~ObjectGroup()
{
	//TODO: should this free the bodies here ?
}