#include "Background.hpp"
#include <core/Common/TrikytaEngine.h>

Background::Background()
{
	m_CurrentLayer = 0;
}

Background::~Background()
{
	for (auto itr : m_Layers) {
		FREE(itr);
	}
	for (auto itr : m_DynamicLayers) {
		FREE(itr);
	}
}

Background& Background::Add(Sprite* sprt, ParralaxType type)
{
	type == STATIC ? m_Layers.emplace_back(sprt) : m_DynamicLayers.emplace_back(sprt);
	return *this;
}

void Background::SetEffectValue(const Vec2i& effect)
{
	m_EffectValue = effect;
}

void Background::Refresh(float dt)
{
	if (m_DynamicLayers.size() == 0) { return; }
	auto sprt = m_DynamicLayers.at(m_CurrentLayer);
	sprt->setPosition(sprt->getPosition() + m_EffectValue);
	Vec2i pos = sprt->getPosition();
	if (pos.x > ENGINE->GetScreenWidth() || pos.y > ENGINE->GetScreenHeight())
	{
		m_CurrentLayer++;
		if (m_CurrentLayer > (int)m_DynamicLayers.size() - 1) {
			m_CurrentLayer = 0;
		}
		auto sprt = m_DynamicLayers.at(m_CurrentLayer);
		sprt->setPosition(Vec2i(-ENGINE->GetScreenWidth(), 0));
	}
	if (m_Clear) {
		Background::Clear();
		m_Clear = false;
	}
}

void Background::Clear()
{
	for (auto itr : m_Layers) {
		FREE(itr);
	}
	for (auto itr : m_DynamicLayers) {
		FREE(itr);
	}
	m_Layers.clear();
	m_DynamicLayers.clear();
}

void Background::QueueClear()
{
	m_Clear = true;
}