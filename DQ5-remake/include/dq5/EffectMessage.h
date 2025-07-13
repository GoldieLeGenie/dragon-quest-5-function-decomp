#pragma once
#include <cstdint>
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct EffectMessage { // sizeof = 0x20 (32 bytes)

        uint32_t ConditionMsg;        // 0x00
        uint32_t OriginalCondMsg;     // 0x04
        uint32_t PlayerCondMsg;       // 0x08
        uint32_t MonsterCondMsg;      // 0x0C
        uint32_t OverlapMsg;          // 0x10
        uint32_t NotLapMsg;           // 0x14
        uint32_t NaturalClearMsg;     // 0x18
        uint16_t action;              // 0x1C
        uint8_t typeID;               // 0x1E
        uint8_t dummy0;               // 0x1F

        // Déclaration des membres statiques nécessaires pour la lecture Excel
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;
    };

    
}
