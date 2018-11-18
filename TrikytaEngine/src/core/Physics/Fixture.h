#pragma once
#include <vector>
#include <PhysicsEngine/Core/Box2D.h>
#include "core/Common/defines.h"

namespace Physics2D {
	class FixtureDef : public b2FixtureDef {
	public : 
		FixtureDef(BodyType, BodyShape, BodyParams, Vec2f, std::vector<Vec2f>);
	};
}