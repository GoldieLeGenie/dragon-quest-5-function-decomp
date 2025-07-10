#pragma once

#include <cstdint>
#include "ar/BitFlag.h" // ar::Flag32 est supposé être BitFlag<uint32_t>

#pragma pack(push, 4)

namespace status {

    struct alignas(4) StageAttribute {
        ar::Flag32 flag_;                 // 0x00
        char mapName_[32];                // 0x04
        int32_t floorIndex_;              // 0x24
        int32_t normalBtlMapIndex_;       // 0x28
        int32_t eventBtlMapIndex_;        // 0x2C
        char* map_;                       // 0x30
        char btlMapName_[32];             // 0x34
        uint16_t battleBackColor_;        // 0x54
        uint8_t _pad1[2];                 // 0x56 - 0x57
        char* btlMap_;                    // 0x58
        char evBtlMapName_[32];           // 0x5C
        char* evBtlMap_;                  // 0x7C
        char evChapelMapName_[32];        // 0x80
        int32_t mapCorrectIndex_;         // 0xA0
        int32_t shopDataIndex_;           // 0xA4
        int32_t furnitureIndex_;          // 0xA8
        bool timestop_;                   // 0xAC
       
        // Méthodes membres (déclarations uniquement ici)
        static bool isCarriageEnable(const StageAttribute* self);
        static bool isCarriageEnter(const StageAttribute* self);
        static const char* getMapName(const StageAttribute*);
    };
    extern StageAttribute* g_StageAttribute;
} // namespace status

#pragma pack(pop)
