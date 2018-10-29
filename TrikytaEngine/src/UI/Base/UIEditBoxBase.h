#pragma once
#include "UI/UIBase.h"
#include "core/Common/Vec2.h"

namespace UI 
{
	class EditBoxBase : public Base
	{
	public:
		virtual void OnUIFocus(bool, Vec2i) override;
		bool IsEditActive = false;
	};
}