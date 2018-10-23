#include "Font.h"

std::map<std::string, Font*> Font::m_Font_Map;

Font* Font::createOrGetFont(std::string p_FontPath, uint8 p_Size)
{
	if (!m_Font_Map.empty() && m_Font_Map[p_FontPath] != nullptr && m_Font_Map[p_FontPath]->getSize() == p_Size) {
		m_Font_Map[p_FontPath]->m_NumberOfUse++;
		//Log("Requested of font %s with size %d WAS FOUND!", p_FontPath.c_str(), p_Size);
		return m_Font_Map[p_FontPath];
	}
	auto _font = new Font(p_FontPath, p_Size);
	m_Font_Map[p_FontPath] = _font;
	//Log("Creating new font %s with size %d!", p_FontPath.c_str(), p_Size);
	return _font;
}

Font::Font(std::string p_FontPath, uint8 p_Size):
	m_FontPath(p_FontPath), m_Size(p_Size), m_NumberOfUse(1)
{
	m_Font = TTF_OpenFont((m_FontPath).c_str(), p_Size);
}

void Font::Destory()
{
	this->m_NumberOfUse--;
	//Log("Number of use of font %s with size %d is == %d", this->m_FontPath.c_str(), this->m_Size, this->m_NumberOfUse);
	if (m_NumberOfUse <= 0) {
		//Log("deleting font %s with size %d!", this->m_FontPath.c_str(), this->m_Size);
		//Font::~Font();
	}
}

Font::~Font()
{
	this->m_NumberOfUse--;
	//Log("Number of use of font %s with size %d is == %d", this->m_FontPath.c_str(), this->m_Size, this->m_NumberOfUse);
	if (m_NumberOfUse <= 0) {
		//Log("deleting font %s with size %d!", this->m_FontPath.c_str(), this->m_Size);
		//Font::~Font();
		TTF_CloseFont(m_Font);
		m_Font_Map[m_FontPath] = nullptr;
	}
}

TTF_Font* Font::getFont() const
{
	return m_Font;
}

std::string  Font::getFontPath() const
{
	return m_FontPath;
}

uint8 Font::getSize() const
{
	return m_Size;
}