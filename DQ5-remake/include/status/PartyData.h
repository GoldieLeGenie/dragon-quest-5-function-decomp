#pragma once

#include "ar/Flag8.h"
#include "dq5/Sex.h"
#include "status/BaseStatus.h"
#include "status/HaveItem.h"
#include "status/HaveAction.h"
#include "status/HaveEquipment.h"
#include <cstdint>

namespace status {

struct alignas(4) PlayerData {
    uint8_t index_;                 // 0x000
    uint8_t kindIndex_;            // 0x001
    uint16_t nameId_;              // 0x002
    char name_[25];                // 0x004 - 0x01C
    dq5::Sex nameBasedSex_;        // 0x020
    uint8_t command_;              // 0x024
    uint8_t actionCursorIndex_;    // 0x025
    ar::Flag8 flag_;               // 0x026
    BaseStatus baseStatus_;       // 0x028 - 0x03B
    HaveItem haveItem_;           // 0x03C - 0x077
    HaveAction haveAction_;       // 0x078 - 0x0E9
    uint32_t nextLevelExp_;       // 0x0EC
    HaveEquipment haveEquipment_; // 0x0F0 - 0x10F
    bool cursedName_;             // 0x110
};

static_assert(sizeof(PlayerData) == 0x114, "PlayerData must be exactly 0x114 bytes.");

} // namespace status
