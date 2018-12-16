#pragma once
#include "UI/UIBase.h"
#include "core/Common/Vec2.h"

namespace UI 
{
	class Text;
	class EditBoxBase : public Base
	{
	public:
		virtual ~EditBoxBase() {}
		virtual void OnUIFocus(bool, const Vec2i&) override;
		virtual void OnToggleEdit(bool) {}
		bool IsEditActive = false;
		UI::Text* getText() { return m_InputText; }
		virtual Vec2i getTextSize();
		virtual Vec2i getTextPos();
	protected:
		UI::Text* m_InputText;
	};
}