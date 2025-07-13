#pragma once
#include "ar/BitFlag.h"

namespace status {

    struct ItemData {
        int index_;
        uint8_t count_;
        ar::Flag8 flag_;
        static bool isSpell(ItemData* self);
        static bool isEquipment(ItemData* self);
        static void setEquipment(ItemData* self, bool flag);
        static void setSpell(ItemData* self, bool flag);
        static void clear(ItemData* self);
        ~ItemData() = default;
        ItemData();
    };
} 