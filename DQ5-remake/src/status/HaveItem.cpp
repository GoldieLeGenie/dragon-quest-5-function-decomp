#include "status/HaveItem.h"
#include "status/BaseHaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"

char status::HaveItem::sortEnable_ = 0;

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

int status::HaveItem::VHaveItemAdd(int itemIndex)
{
    return status::BaseHaveItem::addOne(this, itemIndex);
}

bool status::HaveItem::isSpace(HaveItem* self)
{
    return status::BaseHaveItem::getCount(self) < 12;
}


bool status::HaveItem::isSpell(const HaveItem* self) {
    if (self->itemMax_ < 1) return false;

    for (int i = 0; i < self->itemMax_; ++i) {
        if (status::ItemData::isSpell(&self->item_[i])) {
            return true;
        }
    }
    return false;
}

void status::HaveItem::breakSpell(status::HaveItem* self) {
    for (int i = 0; i < self->itemMax_; ++i) {
        status::ItemData::setSpell(&self->item_[i], 0);
    }
}

void status::HaveItem::sortEquipment(HaveItem* self) {
    if (!self->status::HaveItem::sortEnable_)
        return;

    status::ItemData sortedItems[12];
    int count = 0;

    // Arme
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0
            && status::ItemData::isEquipment(&self->item_[i])
            && status::UseItem::getEquipItemType(self->item_[i].index_) == dq5::level::ItemType::ITEM_WEAPON)
        {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Armure
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0
            && status::ItemData::isEquipment(&self->item_[i])
            && status::UseItem::getEquipItemType(self->item_[i].index_) == dq5::level::ItemType::ITEM_ARMOR)
        {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Bouclier
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0
            && status::ItemData::isEquipment(&self->item_[i])
            && status::UseItem::getEquipItemType(self->item_[i].index_) == dq5::level::ItemType::ITEM_SHIELD)
        {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Casque
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0
            && status::ItemData::isEquipment(&self->item_[i])
            && status::UseItem::getEquipItemType(self->item_[i].index_) == dq5::level::ItemType::ITEM_HELMET)
        {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Accessoire
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0
            && status::ItemData::isEquipment(&self->item_[i])
            && status::UseItem::getEquipItemType(self->item_[i].index_) == dq5::level::ItemType::ITEM_ACCESSORY)
        {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Le reste (non équipements ou autres)
    for (int i = 0; i < 12; ++i) {
        if (self->item_[i].index_ != 0) {
            sortedItems[count++] = self->item_[i];
            self->item_[i].index_ = 0;
        }
    }

    // Recopier dans self->item_
    for (int i = 0; i < count; ++i) {
        self->item_[i] = sortedItems[i];
    }

    // Vider le reste
    for (int i = count; i < 12; ++i) {
        self->item_[i].index_ = 0;
    }
}


void status::HaveItem::cleanup(HaveItem* self, int index)
{
   /*to do*/
}

