#pragma once
#include <core/Common//TrikytaEngine.h>
#include <core/Common/EngineInstance.h>
#include <core/Common/Vec2.h>
#include "Drawable.h"

class Sprite : public Drawable
{
protected:
	Sprite(const std::string& p_Filename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, bool r=true) :
		Drawable(p_Position, p_Size, r), m_Filename(p_Filename)
	{
		init();
	}
public:
	virtual ~Sprite();
	virtual bool init() override;

	static Sprite* Create(const std::string& p_Filename, const Vec2<int>& p_Size, const Vec2<int>& p_Position, bool r=true){
		return new Sprite(p_Filename, p_Size, p_Position, r);
	}

	/*GETTERS!*/
	inline std::string getFileName() const { return m_Filename; }
protected:
	std::string m_Filename;
private:
};