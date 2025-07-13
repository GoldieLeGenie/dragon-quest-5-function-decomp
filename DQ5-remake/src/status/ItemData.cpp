#include "status/ItemData.h"
#include <cstring>


bool status::ItemData::isSpell(ItemData* self) {
    return self->flag_.test(1);
}

void status::ItemData::setEquipment(ItemData* self, bool flag) {
    if (flag)
        self->flag_.set(0);
    else
        self->flag_.reset();
}

void status::ItemData::setSpell(ItemData* self, bool flag)
{
    if (flag)
        self->flag_.set(1);     // Bit 1 = spell
    else
        self->flag_.reset();
}

bool status::ItemData::isEquipment(ItemData* self)
{
    return self->flag_.test(1);
}

void status::ItemData::clear(ItemData* self)
{
    std::memset(self, 0, sizeof(ItemData));
}

status::ItemData::ItemData()
{
    std::memset(this, 0, sizeof(ItemData));
}

