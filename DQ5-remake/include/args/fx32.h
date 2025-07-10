#pragma once
#include <cstdint>

namespace args {
    using fx32 = int32_t;

    // Bornes utiles si tu veux les exposer
    constexpr fx32 fx32_min = static_cast<fx32>(0x80000001);
    constexpr fx32 fx32_max = static_cast<fx32>(0x7FFFFFFF);
}
