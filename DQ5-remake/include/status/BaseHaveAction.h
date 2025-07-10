#pragma once

#include <cstdint>

namespace status {

    struct BaseHaveAction {
        uint16_t battleAction_;   // 0x00
        uint16_t usuallyAction_;  // 0x02
        uint8_t actionLevel_;     // 0x04
        uint8_t actionFlag_;      // 0x05
    };

} // namespace status
