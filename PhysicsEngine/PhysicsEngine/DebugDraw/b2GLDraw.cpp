#include <iostream>
#include <core/Common/defines.h>
#include <core/Common/TrikytaEngine.h>
#include <core/Common/EngineInstance.h>
#include "b2GLDraw.h"

//http://www.box2d.org/forum/viewtopic.php?f=3&t=1944

b2GLDraw::b2GLDraw()
{
	//ctor
}

b2GLDraw::~b2GLDraw()
{
	//dtor
}

void b2GLDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	auto renderer = ENGINE->getRenderer();
	for (int i = 0; i < vertexCount - 1; i++)
	{
		auto vec1 = b2Vec2(vertices[i].x, vertices[i].y);
		auto vec2 = b2Vec2(vertices[i+1].x, vertices[i+1].y);
		lineRGBA(ENGINE->getRenderer(), (Sint16)vec1.x, (Sint16)vec1.y, (Sint16)vec2.x, (Sint16)vec2.y,255, 0, 0, 125);
	}
	int i = vertexCount-1;
	auto vec1 = b2Vec2(vertices[i].x, vertices[i].y);
	auto vec2 = b2Vec2(vertices[0].x, vertices[0].y);
	lineRGBA(ENGINE->getRenderer(), (Sint16)vec1.x, (Sint16)vec1.y, (Sint16)vec2.x, (Sint16)vec2.y, 255, 0, 0, 125);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}

void b2GLDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	DrawPolygon(vertices, vertexCount, color);
}

void b2GLDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
	lineRGBA(ENGINE->getRenderer(), (Sint16)p1.x, (Sint16)p1.y, (Sint16)p2.x, (Sint16)p2.y, 0, 0, 255, 255);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
}
void b2GLDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color)
{
	circleRGBA(ENGINE->getRenderer(), (Sint16)center.x, (Sint16)center.y, (Sint16)radius, 0, 255, 255, 255);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
}

void b2GLDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
	circleRGBA(ENGINE->getRenderer(), (Sint16)center.x, (Sint16)center.y, (Sint16)radius, 0, 255, 255, 255);
	SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
}

void b2GLDraw::DrawAABB(b2AABB* aabb, const b2Color& c) 
{
	b2GLDraw::DrawSegment(b2Vec2(aabb->lowerBound.x * 30, aabb->lowerBound.y * 30), b2Vec2(aabb->upperBound.x * 30, aabb->lowerBound.y * 30), b2Color());
	b2GLDraw::DrawSegment(b2Vec2(aabb->upperBound.x * 30, aabb->upperBound.y*30), b2Vec2(aabb->lowerBound.x * 30, aabb->upperBound.y * 30), b2Color());
}

void b2GLDraw::DrawTransform(const b2Transform& xf) {

	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.0f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1, p2, b2Color(1, 0, 0));

	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1, p2, b2Color(0, 1, 0));
}
