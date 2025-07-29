#pragma once
#include <cstdint>
#include "status/HaveItem.h"
#include "dq5/ItemType.h"

namespace status {

    struct HaveEquipment {
        int playerIndex_;                   // 0x00
        int calcNoEquipmentItemIndex_;     // 0x04
        int calcEquipmentItemIndex_;   
        uint8_t strength_;                 // 0x0C
        uint8_t agility_;                  // 0x0D
        uint8_t protection_;              // 0x0E
        uint8_t wisdom_;                  // 0x0F
        uint8_t luck_;                    // 0x10

        uint16_t hp_;                     // 0x12
        uint16_t mp_;                     // 0x14
        uint16_t attack_;                 // 0x16
        uint16_t defence_;                // 0x18
                     
        HaveItem* haveItem_;             // 0x1C
        static bool isEquipment(const HaveEquipment* self, int itemIndex);
        static void calcEffect(HaveEquipment* self);
        static void calcEffect(HaveEquipment* self, int itemIndex, dq5::level::ItemType type);
        static int getEquipment(const HaveEquipment* self, dq5::level::ItemType type, int index);
        static void resetEquipment(HaveEquipment* self, int itemIndex);
        static void clear(HaveEquipment* self);
        static void setup(HaveEquipment* self, HaveItem* haveItem);
        static void setup(HaveEquipment* self, int index);
        ~HaveEquipment();
        HaveEquipment() {
            strength_ = attack_ = 0;
        };

     
    };

} 
