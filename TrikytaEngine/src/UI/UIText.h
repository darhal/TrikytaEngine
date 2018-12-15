#pragma once
#include "core/Common/defines.h"
#include "core/Drawable/Drawable.h"
#include "misc/Font.h"
#include "core/Common/Color.h"
#include "UIBase.h"

namespace UI {
	class EditBox;

	class Text : public Base,public Drawable
	{
	public:
		static Text* createText(const std::string& p_Text, const std::string& p_Font, uint8 p_TextSize, const Vec2i& p_Pos,const Color& p_Color, int p_FontStyle = Font::Style::NORMAL, bool p_Register = true)
		{
			return new Text(p_Text, p_Font, p_TextSize, p_Pos, p_Color, p_FontStyle, p_Register);
		}

		static Text* createText(const std::string& p_Text, Font* p_Font, const Vec2i& p_Pos,const Color& p_Color, bool p_Register = true)
		{
			return new Text(p_Text, p_Font, p_Pos, p_Color, p_Register);
		}

		~Text();

		void updateText(const std::string&);
		void setScale(uint8);
		void setColor(Color);
		const std::string& getText() const { return m_Text; }
		void setBackgroundColor(Color);
		void setTextStyle(::Font::Style);
		void setTextStyle(int);
		Font* getFont() {return m_Font;}
		virtual Vec2i getPos() override { return m_Position; };
		virtual Vec2i getSize() override { return m_Size; };
		virtual void setPos(const Vec2i& p) { Drawable::setPosition(p); };
		virtual void render(float dt) override { Drawable::render(dt); };
	protected:
		Text(const std::string&, const std::string&, uint8, const Vec2i&, const Color&, int = Font::Style::NORMAL, bool=true);
		Text(const std::string&, Font*,const Vec2i&, const Color&, bool = true);
		bool init();
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