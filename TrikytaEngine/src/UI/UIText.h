#pragma once
#include "core/Drawable/Drawable.h"
#include <SDL/SDL_ttf.h>

typedef unsigned char uint8;

struct Color {
	uint8 r, g, b, a;
	Color(uint8 r, uint8 g, uint8 b, uint8 a) :
		r(r), g(g), b(b), a(a)
	{}
	
};

namespace UI {
	class Text : public Drawable
	{
	public:
		static Text* createText(std::string p_Text, std::string p_Font, uint8 p_TextSize, Vec2i p_Pos, Color p_Color)
		{
			return new Text(p_Text, p_Font, p_TextSize, p_Pos, p_Color);
		}

		void UpdateText(std::string);
	protected:
		Text(std::string, std::string, uint8, Vec2i, Color);

		virtual bool init() override;
		void render(float) override;
	private:
		SDL_Texture* m_TextTexture;
		std::string& m_FontPath;
		std::string m_Text;
		TTF_Font* m_Font;
		uint8 m_TextSize;
		Color m_Color;

	};
}