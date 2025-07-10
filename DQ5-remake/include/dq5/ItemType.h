#pragma once
#include <cstdint>

namespace dq5::level {

    enum class ItemType : int32_t {
        ITEM_WEAPON = 0x0,
        ITEM_ARMOR = 0x1,
        ITEM_SHIELD = 0x2,
        ITEM_HELMET = 0x3,
        ITEM_ACCESSORY = 0x4,
        ITEM_ITEM = 0x5,
        ITEM_SEED = 0x6,
        ITEM_KEY = 0x7,
        ITEM_CASINO = 0x8,
        ITEM_OTHER = 0x9,
        ITEM_IMPORTANT = 0xA,
        ITEM_SPECIALTY = 0xB,
        ITEM_DEBUG = 0xC
    };

} // namespace dq5
