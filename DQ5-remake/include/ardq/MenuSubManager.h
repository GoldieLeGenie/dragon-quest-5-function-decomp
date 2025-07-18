#pragma once 
#include "ardq/MenuBase.h"

namespace ardq {
	struct MenuSubManager
	{
		bool m_update;
		ardq::MenuBase* m_menu[8];
		ardq::MenuBase* m_next[8];
		ardq::MenuBase* menu_[8];
		ardq::MenuBase* next_[8];
		bool update_;
		static bool isOpen(MenuSubManager* self, ardq::MenuBase* menu);
		static MenuSubManager* s_currentMenu;

	};
	
}
