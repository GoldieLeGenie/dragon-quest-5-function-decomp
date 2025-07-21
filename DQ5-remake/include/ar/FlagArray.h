#pragma once
#include "ar/BitFlag.h"
#include <cstddef> // pour std::size_t

namespace ar {
	template <std::size_t N>
	struct FlagArray {
		ar::Flag32 flag_[7];
	};
}