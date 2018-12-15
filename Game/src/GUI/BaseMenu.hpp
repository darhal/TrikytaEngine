#pragma once
#include <core/Common/defines.h>

class BaseMenu
{
public:
	BaseMenu(class GUIManager* manager):GUI_Manager(manager){}
	virtual ~BaseMenu() {};
protected:
	class GUIManager* GUI_Manager;
};