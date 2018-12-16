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

class Camera;

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
	inline void setSize(const Vec2i& p_Size) { m_Size = p_Size; updateRenderSize(); }
	inline void setPosition(const Vec2i& p_Position) { m_Position = p_Position; if (!isAffectedByCamera()) updateRenderPosition(m_Position); }
	inline void setPositionX(int x) { m_Position.x = x; if (!isAffectedByCamera()) updateRenderPosition(m_Position); }
	inline void setPositionY(int y) { m_Position.y = y; if (!isAffectedByCamera()) updateRenderPosition(m_Position); }
	inline Vec2i* getVecPosPtr() {return &m_Position;}
	virtual Vec2i getSize() const { return m_Size; }
	virtual Vec2i getPosition() override { return m_Position; }

	inline virtual void updateRenderPositionFromCamera(Vec2i p_pos)
	{
		updateRenderPosition(getPosition() - p_pos);
	}

	inline void updateRenderSource(Vec2i p_src) {
		m_SourceDrawCoord.x = p_src.x;
		m_SourceDrawCoord.y = p_src.y;
	}

	//ATTACH FUNCTION:
	virtual void attachTo(Drawable*, Vec2f);
	void attachTo(class Physics2D::PhysicsBody*, Vec2f);
	void ToggleRotationAttachement(bool);

	virtual void setAffectedByCamera(Camera* m_cam) { m_Camera = m_cam; }
	bool isAffectedByCamera() const { return m_Camera != nullptr; }
	Camera* getCamera() { return m_Camera; }
	void render(float);

	class Physics2D::PhysicsBody* Physicalize(struct Physics2D::BodyParams p_BodyParam, enum class Physics2D::BodyType, enum class Physics2D::BodyShape, Vec2f = Vec2f(0.0f, 0.0f));
	virtual ~Drawable();
protected:
	Drawable(const Vec2i& m_Pos = Vec2i(0, 0), const Vec2i& p_Size = Vec2i(0, 0), bool p_RegisterInHandler=true);
	Drawable(bool p_RegisterInHandler = true);

	inline void updateRenderPosition(Vec2i p_pos)
	{
		m_DestinationDrawCoord.x = p_pos.x;
		m_DestinationDrawCoord.y = p_pos.y;
	}

	inline void updateRenderSize() {
		m_DestinationDrawCoord.w = m_Size.x;
		m_DestinationDrawCoord.h = m_Size.y;
	}

	double m_Angle;
	SDL_Point m_RotationCenter;
	SDL_RendererFlip m_Flip;
	Vec2i m_Size;
	Vec2i m_Position;
	SDL_Texture* m_Texture;
	Vec2i m_NormalSize; // The size of the sprite to draw!
	Vec2i m_DrawCoord;
	SDL_Rect m_DestinationDrawCoord;
	SDL_Rect m_SourceDrawCoord;
	Vec2i m_ToFollowPos;
	Vec2i m_Offset;
	Camera* m_Camera;
	bool m_IsRotationAttached;
	int m_ZOrder;

	class Physics2D::PhysicsBody* m_Body = nullptr;

	friend class TiledMap;
private:
	
};
