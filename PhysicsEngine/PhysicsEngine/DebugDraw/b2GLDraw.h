#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "../Core/Box2d.h"
#include <utils/SDL2_gfxPrimitives.h>

class b2GLDraw : public b2Draw
{
public:
	b2GLDraw();
	~b2GLDraw();

	void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
	void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
	void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) override;
	void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) override;
	void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf);
	void DrawAABB(b2AABB* aabb, const b2Color& c);
	/// Draw a point.
	virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {};
	//void DrawXForm(const b2XForm &xf);
protected:
private:
};

#endif // DEBUGDRAW_H
