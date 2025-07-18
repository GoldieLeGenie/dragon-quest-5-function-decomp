#pragma once
#include "ar/BitFlag.h"

namespace status {
    struct GameFlag {
        ar::Flag32 flag_[32];  // 32 flags = 128 octets
        static bool check(GameFlag* self, uint32_t index);
        void clear(GameFlag* self);
        void initialize(GameFlag* self);
        void operator=(GameFlag* rhs);
        void remove(GameFlag* self, uint32_t index);
        void set(GameFlag* self, uint32_t index);
        GameFlag();
        ~GameFlag();
    };
    extern GameFlag g_GlobalFlag;
}