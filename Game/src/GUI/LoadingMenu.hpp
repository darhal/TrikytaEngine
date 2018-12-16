#pragma once
#include "BaseMenu.hpp"
#include <core/Common/defines.h>

class LoadingMenu : public BaseMenu
{
public:
	LoadingMenu(class GUIManager*); 
	~LoadingMenu();
	void BuildLoadingMenu();
	void AddProgress(int);
private:
	UI::Progressbar* m_ProgressBar;
};