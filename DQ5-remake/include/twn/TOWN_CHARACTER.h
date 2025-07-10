#pragma once
#include "ar/Fix32Vector3.h"
#include "ar/BitFlag.h"

namespace twn {
	struct TOWN_CHARACTER
	{
		bool enable;
		int charaIndex;
		int dir;
		int ctrlNo;
		ar::Fix32Vector3 position;
		ar::Flag32 flag_;
	};
}