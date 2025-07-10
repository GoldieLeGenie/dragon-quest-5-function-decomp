#pragma once

#include <cstdint>

namespace dq5::level {

    enum class CommandTypeDebug : int32_t {
        DEFENCE_ONLY = 0,
        KONRAN_ONLY = 1,
        ATTACK_100 = 2,
        ATTACK_50 = 3,
        ATTACK_20 = 4,
        PARUPUNTE_ONLY = 5
    };

}
