#include "status/HaveItem.h"
#include "status/BaseHaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"


bool status::HaveItem::isEquipmentInAllItem(const HaveItem* self, int itemIndex) {
    int count = status::BaseHaveItem::getCount(self);
    if (count < 1)
        return false;

    for (int i = 0; i < count; ++i) {
        if (status::BaseHaveItem::isEquipment(self,i) && status::BaseHaveItem::getItem(self, i) == itemIndex) {
            return true;
        }
    }
    return false;
}

int status::HaveItem::getEquipmentItem(HaveItem* self,dq5::level::ItemType type, int index) {
    int count = status::BaseHaveItem::getCount(self);  
    if (count < 1) return 0;

    int current = 0;
    for (int i = 0; i < count; ++i) {
        if (status::BaseHaveItem::isEquipment(self,i)) {
            int itemId = status::BaseHaveItem::getItem(self,i);
            if (status::UseItem::getEquipItemType(itemId) == type) {
                if (current == index)
                    return itemId;
                ++current;
            }
        }
    }
    return 0;
}

int status::HaveItem::add(BaseHaveItem* self, int itemIndex)
{
    return status::BaseHaveItem::addOne(self, itemIndex);
}

