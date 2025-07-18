#pragma once
#include "ardq/MenuBase.h"

namespace ardq {
	struct MenuManager {
		static bool isOpenMenu(ardq::MenuBase* menu);
	};
}