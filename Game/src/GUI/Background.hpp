#pragma once
#include <vector>
#include <core/Drawable/Sprite.h>

enum ParralaxType {
	STATIC,
	DYNAMIC,
};

class Background
{
public:
	Background();
	~Background();
	Background& Add(Sprite*, ParralaxType);
	void Refresh(float);
	void SetEffectValue(const Vec2i& effect);
	void Clear();
	void QueueClear();
protected:

private:
	std::vector<Sprite*> m_Layers;
	std::vector<Sprite*> m_DynamicLayers;
	Vec2i m_EffectValue;
	int m_CurrentLayer;
	bool m_Clear;
};