#pragma once
#include "status/BaseHaveItem.h"
#include "status/ItemData.h"
#include "dq5/ItemType.h"

namespace status {

    struct HaveItem : public BaseHaveItem {
        ItemData itemArray_[12];  // 0x0C : Tableau des objets possédés (12 objets max)
        static bool isEquipmentInAllItem(const HaveItem* self, int itemIndex);
        static int getEquipmentItem(HaveItem* self, dq5::level::ItemType type, int index);
        static int add(BaseHaveItem* self, int itemIndex);
        static bool isSpace(HaveItem* self);
        static bool isSpell(const HaveItem* self);
        static void breakSpell(HaveItem* self);
        static void sortEquipment(HaveItem* self);
        static char sortEnable_;
        static void cleanup(HaveItem* self, int index);
        // Constructeurs
        HaveItem() {
            item_ = itemArray_;
            itemMax_ = 12;
      
        }

        // Destructeur
        ~HaveItem() = default;
    };

} // namespace status
