#pragma once
#include <core/Common//TrikytaEngine.h>
#include <core/Common/EngineInstance.h>
#include <core/Common/Vec2.h>
#include "Drawable.h"

class Sprite : public Drawable
{
protected:

	Sprite(const std::string& p_Filename, Vec2<int> p_Size, Vec2<int> p_Position) :
		Drawable(p_Position, p_Size), m_Filename(p_Filename)
	{
		init();
	}

	Sprite(const std::string& p_Filename, Vec2<int>& p_Position) :
		m_Filename(p_Filename), Drawable(p_Position)
	{
		init();
	}

	/*Sprite(std::string p_Filename, Vec2<int> p_Size) :
		m_Filename(p_Filename), Drawable(Vec2i(0,0), p_Size)
	{
		init();
	}*/

public:
	virtual ~Sprite();
	virtual bool init() override;

	static Sprite* Create(const std::string& p_Filename, Vec2<int> p_Size, Vec2<int> p_Position)
	{
		return new Sprite(p_Filename, p_Size, p_Position);
	}

	/*GETTERS!*/
	inline std::string getFileName() const { return m_Filename; }
protected:
	std::string m_Filename;
};