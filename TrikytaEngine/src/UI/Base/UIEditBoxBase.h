#pragma once
#include "UI/UIBase.h"

namespace UI 
{
	class EditBoxBase : public Base
	{
	public:
		virtual void OnUIFocus(bool) override;
		bool IsEditActive = false;
	};
}