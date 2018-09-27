#pragma once
#include <core/Objects/Object.h>
#include "core/Common/defines.h"
#include <SDL/SDL.h>
#include <memory>

enum class FLIPTYPE {
	HORIZONTAL = SDL_RendererFlip::SDL_FLIP_HORIZONTAL,
	VERTICAL = SDL_RendererFlip::SDL_FLIP_VERTICAL,
	NONE = SDL_RendererFlip::SDL_FLIP_NONE
};

class Drawable : public Object
{
public:
	void SortZOrder();
	void setZOrder(int p_ZOrder);
	int getZOrder() const;
	Physics2D::PhysicsBody* getBody();

	// SET GET ROT FUNCTIONS
	virtual double GetRotation() override { return m_Angle; }

	inline void Flip(const FLIPTYPE& p_Type)
	{
		m_Flip = (SDL_RendererFlip)p_Type;
	}

	inline void Rotate(double p_Angle)
	{
		m_Angle = p_Angle;
	}

	inline void SetRotationCenter(const Vec2i& p_rotCenter)
	{
		m_RotationCenter = SDL_Point{ p_rotCenter.x, p_rotCenter.y };
	}
	
	//GET SET DRAW SIZE POS
	inline void setNormalSize(const Vec2i& p_Size) { m_NormalSize = p_Size; updateNormalSizeCoord(); }
	inline void setDrawCoordconst (Vec2i& p_DrawCoord) { m_DrawCoord = p_DrawCoord; updateSourceDrawCoord(); }
	inline void updateSourceDrawCoord() {
		m_SourceDrawCoord.x = m_DrawCoord.x;
		m_SourceDrawCoord.y = m_DrawCoord.y;
	};

	inline void updateNormalSizeCoord(){
		m_SourceDrawCoord.w = m_NormalSize.x;
		m_SourceDrawCoord.h = m_NormalSize.y;
	}

	// SIZE POS FUNCTIONS
	inline void setSize(const Vec2i& p_Size) { *m_Size = p_Size; updateSize(); }
	inline void setPosition(const Vec2i& p_Position) { *m_Position = p_Position; updatePosition(); }
	inline void setPositionX(int x) { m_Position->x = x; updatePosition(); }
	inline void setPositionY(int y) { m_Position->y = y; updatePosition(); }
	inline void followPosition(Vec2i* v) { FREE(m_Position); m_Position = v; }
	inline Vec2i* getVecPosPtr() {return m_Position;}
	virtual Vec2i& getSize() const { return *m_Size; }
	virtual Vec2i& getPosition() const { return *m_Position; }
	virtual Vec2i getPosition() override { return *m_Position; }

	//ATTCH FUNCTION:
	virtual void attachTo(Drawable*, Vec2f);
	void attachTo(class Physics2D::PhysicsBody*, Vec2f);


	void render(float);

	class Physics2D::PhysicsBody* Physicalize(float, float, enum class Physics2D::BodyType, Vec2f = Vec2f(0.0f, 0.0f));
	~Drawable();
protected:
	Drawable(Vec2i m_Pos = Vec2i(0, 0), Vec2i p_Size = Vec2i(0, 0), bool p_RegisterInHandler=true);

	inline void updatePosition() {
		m_DestinationDrawCoord.x = m_Position->x;
		m_DestinationDrawCoord.y = m_Position->y;
	}

	inline void updateSize() {
		m_DestinationDrawCoord.w = m_Size->x;
		m_DestinationDrawCoord.h = m_Size->y;
	}


	double m_Angle;
	SDL_Point m_RotationCenter;
	SDL_RendererFlip m_Flip;
	Vec2i* m_Size;
	Vec2i* m_Position;
	SDL_Texture* m_Texture;
	Vec2i m_NormalSize; // The size of the sprite to draw!
	Vec2i m_DrawCoord;
	SDL_Rect m_DestinationDrawCoord;
	SDL_Rect m_SourceDrawCoord;
	Vec2i m_ToFollowPos;
	Vec2i m_Offset;
	int m_ZOrder;

	class Physics2D::PhysicsBody* m_Body = nullptr;
private:

};