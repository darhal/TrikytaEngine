#pragma once
#include "core/Common/defines.h"
#include "core/Drawable/Drawable.h"
#include "misc/Font.h"
#include "core/Common/Color.h"

namespace UI {
	class Text : public Drawable
	{
	public:
		static Text* createText(std::string p_Text, std::string p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color, bool p_Register=true)
		{
			return new Text(p_Text, p_Font, p_TextSize, p_Pos, p_Color, p_Register);
		}

		~Text();

		void updateText(std::string);
		void setScale(uint8);
		void setColor(Color);
	protected:
		Text(std::string, std::string, uint8, Vec2i, Color, bool=true);
		virtual bool init() override;
	private:
		class Font* m_Font;
		std::string& m_FontPath;
		std::string m_Text;
		uint8 m_TextSize;
		Color m_Color;
		uint8 m_Scale;
	};
}