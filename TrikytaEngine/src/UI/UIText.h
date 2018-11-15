#pragma once
#include "core/Common/defines.h"
#include "core/Drawable/Drawable.h"
#include "misc/Font.h"
#include "core/Common/Color.h"

namespace UI {
	class EditBox;

	class Text : public Drawable
	{
	public:
		static Text* createText(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color, int p_FontStyle = Font::Style::NORMAL, bool p_Register = true)
		{
			return new Text(p_Text, p_Font, p_TextSize, p_Pos, p_Color, p_FontStyle, p_Register);
		}

		~Text();

		void updateText(const std::string&);
		void setScale(uint8);
		void setColor(Color);
		std::string getText() const { return m_Text; }
		void setBackgroundColor(Color);
		void setTextStyle(::Font::Style);
		void setTextStyle(int);
		Font* getFont() {return m_Font;}
	protected:
		Text(const std::string&, const std::string&, uint8, Vec2i, Color, int = Font::Style::NORMAL, bool=true);
		virtual bool init() override;
		void updateTextHelper();
	private:
		class Font* m_Font;
		std::string m_FontPath;
		std::string m_Text;
		uint8 m_TextSize;
		Color m_Color;
		Color m_BGColor;
		uint8 m_Scale;
		int m_Style;
		friend class EditBox;
	};
}