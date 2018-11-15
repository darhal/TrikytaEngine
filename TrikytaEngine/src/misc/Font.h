#pragma once

#include <SDL/SDL_ttf.h>
#include <string>
#include <map>
#include "core/Common/defines.h"

class Font
{
public:
	enum Style {
		BOLD = TTF_STYLE_BOLD,
		ITALIC = TTF_STYLE_ITALIC,
		UNDERLINE = TTF_STYLE_UNDERLINE,
		STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH,
		NORMAL = TTF_STYLE_NORMAL
	};

	static Font* createOrGetFont(std::string, uint8);

	Font(std::string, uint8);
	~Font();
	void Destory();

	TTF_Font* getFont() const;
	std::string getFontPath() const;
	uint8 getSize() const;

	void setTextStyle(Style style)
	{
		TTF_SetFontStyle(m_Font, style);
	}

	void setTextStyle(int style)
	{
		TTF_SetFontStyle(m_Font, style);
	}
protected:
	uint8 m_Size;
	TTF_Font* m_Font;
	std::string m_FontPath;
	uint32 m_NumberOfUse;
private:
	static std::map<std::string, Font*> m_Font_Map;
};