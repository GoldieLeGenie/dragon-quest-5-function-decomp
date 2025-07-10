#pragma once
#include <cstdint>
#include "ar/BitFlag.h" // Assurez-vous que la classe ar::Flag est bien incluse

namespace status {

    struct alignas(4) StatusChangeOne {
        ar::Flag32 flag_;         // 0x00
        int16_t actionIndex_;   // 0x04
        uint8_t damageType_;    // 0x06
        uint8_t count_;         // 0x07
        uint8_t turn_;          // 0x08
        int8_t pattern_;        // 0x09
        int8_t astron_;         // 0x0A
        uint8_t padding_ = 0;   // 0x0B (remplissage pour alignement si nécessaire)
        static bool isEnable(const StatusChangeOne* self);
    };


} // namespace status
