#pragma once

#include <SDL/SDL_ttf.h>
#include <string>
#include <map>
#include "core/Common/defines.h"

class Font
{
public:
	static Font* createOrGetFont(std::string, uint8);

	Font(std::string, uint8);
	~Font();
	void Destory();

	TTF_Font* getFont() const;
	std::string getFontPath() const;
	uint8 getSize() const;
protected:
	uint8 m_Size;
	TTF_Font* m_Font;
	std::string m_FontPath;
	uint32 m_NumberOfUse;
private:
	static std::map<std::string, Font*> m_Font_Map;
};