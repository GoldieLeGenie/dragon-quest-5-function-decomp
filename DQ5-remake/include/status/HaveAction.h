#pragma once

#include <cstdint>
#include "status/BaseHaveAction.h"

namespace status {

    class HaveAction {
    public:
        BaseHaveAction action_[18];  // 0x00–0x6B
        uint16_t index_;             // 0x6C
        uint16_t kind_;              // 0x6E
        uint16_t level_;             // 0x70

        static bool isBattleMode();
        static void setMode(int value);  

    private:
        static int mode_;
    };

} 