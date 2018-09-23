#pragma once
#include <core/Objects/Object.h>
#include <core/Common//TrikytaEngine.h>
#include <core/Common/EngineInstance.h>
#include <core/Common/Vec2.h>

enum class FLIPTYPE {
	HORIZONTAL = SDL_RendererFlip::SDL_FLIP_HORIZONTAL,
	VERTICAL = SDL_RendererFlip::SDL_FLIP_VERTICAL,
	NONE = SDL_RendererFlip::SDL_FLIP_NONE
};

class Sprite : public Object
{
protected:

	Sprite(std::string p_Filename, Vec2<int> p_Size, Vec2<int> p_Position) :
		Object(m_Position, m_Size), m_Filename(p_Filename)
	{
		m_Size = p_Size;
		m_Position = p_Position;
		init();
	}
	Sprite(std::string p_Filename, Vec2<int> p_Position) :
		m_Filename(p_Filename)
	{
		m_Size = Vec2i(0, 0);
		m_Position = p_Position;
		init();
	}

public:
	virtual ~Sprite();
	virtual bool init() override;
	virtual void render(float) override;

	static Sprite* Create(const std::string& p_Filename, Vec2<int> p_Size, Vec2<int> p_Position)
	{
		return new Sprite(p_Filename, p_Size, p_Position);
	}

	/*FUNCTIONS*/
	inline void Flip(FLIPTYPE p_Type)
	{
		m_Flip = (SDL_RendererFlip)p_Type;
	}

	inline void Rotate(double p_Angle)
	{
		m_Angle = p_Angle;
	}

	inline void SetRotationCenter(Vec2i p_rotCenter)
	{
		m_RotationCenter = SDL_Point{ p_rotCenter.x, p_rotCenter.y };
	}

	/*GETTERS!*/
	inline std::string getFileName() const { return m_Filename; }
	inline Vec2i getDrawSize() { return Vec2i(m_DestinationDrawCoord.h, m_DestinationDrawCoord.w); }
	inline double GetRotation() { return m_Angle; }

	/*SETTERS*/
	inline void setSize(Vec2<int> p_Size) { m_Size = p_Size; }
	inline void setPosition(Vec2<int> p_Position) { m_Position = p_Position; }
	inline void setNormalSize(Vec2<int> p_Size) { m_NormalSize = p_Size; updateSourceDrawCoord(); }
	inline void setDrawCoord(Vec2<int> p_DrawCoord) { m_DrawCoord = p_DrawCoord; updateSourceDrawCoord(); }
	inline void setPositionX(int x) { m_Position.x = x; }
	inline void setPositionY(int y) { m_Position.y = y; }

	/*Utility*/
	inline void updateSourceDrawCoord() {
		m_SourceDrawCoord.x = m_DrawCoord.x;
		m_SourceDrawCoord.y = m_DrawCoord.y;
		m_SourceDrawCoord.h = m_NormalSize.x;
		m_SourceDrawCoord.w = m_NormalSize.y;

	};
	inline void updateDestinationDrawCoord() {
		m_DestinationDrawCoord.x = m_Position.x;
		m_DestinationDrawCoord.y = m_Position.y;
		m_DestinationDrawCoord.h = m_Size.x;
		m_DestinationDrawCoord.w = m_Size.y;
	}

protected:
	std::string m_Filename;
	SDL_Texture* m_Texture;
	Vec2<int> m_NormalSize; // The size of the sprite to draw!
	Vec2<int> m_DrawCoord;
	SDL_Rect m_SourceDrawCoord;
	SDL_Rect m_DestinationDrawCoord;
	double m_Angle;
	SDL_Point m_RotationCenter;
	SDL_RendererFlip m_Flip;
};