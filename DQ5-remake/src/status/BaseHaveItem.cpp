
#include "status/BaseHaveItem.h"
#include "status/ItemData.h"
#include "status/UseItem.h"
#include <cstring>

int status::BaseHaveItem::addNum(BaseHaveItem* self, int itemIndex, int count)
{
    if (self->itemMax_ < 1 || self->item_ == nullptr)
        return 0;

    for (int i = 0; i < self->itemMax_; ++i)
    {
        ItemData& item = self->item_[i];

        if (item.index_ == itemIndex)
        {
            item.count_ += count; 
            return i;
        }

        if (item.index_ == 0)
        {
            item.index_ = itemIndex;
            item.count_ = count;
            return i;
        }
    }

    return 0; 
}


int status::BaseHaveItem::getMaxCount(const BaseHaveItem* self)
{ 
    return self->itemMax_; 
}

int status::BaseHaveItem::getItem(const BaseHaveItem* self,int index)
{ 
    return self->item_[index].index_; 
}

int status::BaseHaveItem::getItemCount(const BaseHaveItem* self,int index) 
{ 
    return self->item_[index].count_; 
}

int status::BaseHaveItem::getCount(const BaseHaveItem* self){
    if (self->itemMax_ < 1 || self->item_ == nullptr) return 0;
    int result = 0;
    for (int i = 0; i < self->itemMax_; ++i)
        if (self->item_[i].index_ != 0) ++result;
    return result;
}

bool status::BaseHaveItem::isItem(const BaseHaveItem* self, int itemIndex)
{
    if (self->itemMax_ < 1 || self->item_ == nullptr)
        return false;

    for (int i = 0; i < self->itemMax_; ++i)
    {
        if (self->item_[i].index_ == itemIndex)
            return true;
    }

    return false;
}

void status::BaseHaveItem::sort(BaseHaveItem* self) {
    if (self->itemMax_ < 1 || self->item_ == nullptr) return;
    ItemData* temp = new ItemData[self->itemMax_];
    int count = 0;
    for (int i = 0; i < self->itemMax_; ++i)
        if (self->item_[i].index_ != 0)
            temp[count++] = self->item_[i];
    for (int i = 0; i < self->itemMax_; ++i)
        self->item_[i] = ItemData();
    for (int i = 0; i < count; ++i)
        self->item_[i] = temp[i];
    delete[] temp;
}

int status::BaseHaveItem::add(BaseHaveItem* self, int itemIndex)
{
    if (itemIndex == 0)
        return -1; 

    if (self->itemMax_ < 1)
        return 0; 

    int result = 0;

    for (int i = 0; i < self->itemMax_; ++i)
    {
        if (self->item_[i].index_ == 0)
        {
            self->item_[i].index_ = itemIndex;
            self->item_[i].count_ = 1;
            return i;
        }

        ++result;
        if (result >= self->itemMax_)
            return 0;
    }

    return 0;
}


bool status::BaseHaveItem::del(BaseHaveItem* self, int ctrlId) {
    if (ctrlId < 0 || ctrlId >= self->itemMax_ || self->item_ == nullptr) return false;
    self->item_[ctrlId] = ItemData();
    status::BaseHaveItem::sort(self);
    return true;
}

void status::BaseHaveItem::sortEquipment(BaseHaveItem* self) {
}

status::BaseHaveItem::BaseHaveItem() : item_(nullptr), itemMax_(0) {}
status::BaseHaveItem::~BaseHaveItem() {}

status::BaseHaveItem& status::BaseHaveItem::operator=(const BaseHaveItem& rhs) {
    if (item_ && rhs.item_ && itemMax_ >= 1)
        for (int i = 0; i < itemMax_; ++i)
            item_[i] = rhs.item_[i];
    return *this;
}

bool status::BaseHaveItem::delOne(BaseHaveItem* self, int ctrlId) {
    return status::BaseHaveItem::del(self,ctrlId);
}

bool status::BaseHaveItem::isEquipment(const BaseHaveItem* self,int index){
    return (self->item_[index].flag_.get() & 1) != 0;
}

void status::BaseHaveItem::resetEquipment(BaseHaveItem* self, int index) {
    if (index < 0 || index >= self->itemMax_ || self->item_ == nullptr) return;
    self->item_[index].flag_.clear(0);
    status::BaseHaveItem::sortEquipment(self);
}

int status::BaseHaveItem::getItemSortIndex(const BaseHaveItem* self, int itemIndex){
    if (self->itemMax_ < 1 || self->item_ == nullptr) return -1;
    for (int i = 0; i < self->itemMax_; ++i)
        if (self->item_[i].index_ == itemIndex) return i;
    return -1;
}

void status::BaseHaveItem::resetEquipmentWithItemIndex(BaseHaveItem* self, int itemIndex) {
    if (!self || !self->item_ || self->itemMax_ <= 0) return;

    int activeCount = 0;
    for (int i = 0; i < self->itemMax_; ++i) {
        if (self->item_[i].index_ != 0) ++activeCount;
    }

    for (int i = 0; i < activeCount; ++i) {
        if (self->item_[i].index_ == itemIndex && self->item_[i].flag_.test(0)) {
            self->item_[i].flag_.clear(0);

           
        }
    }
}

void status::BaseHaveItem::setEquipment(BaseHaveItem* self, int index) {
    if (index == -1 || !self || !self->item_) return;

    auto& item = self->item_[index];

    item.flag_.set(0);

    if (status::UseItem::isCurse(item.index_)) {
        item.flag_.set(1);
    }

    status::BaseHaveItem::sortEquipment(self);

}


int status::BaseHaveItem::getItemMax(BaseHaveItem* self)
{ 
    return self->itemMax_; 
}


status::ItemData* status::BaseHaveItem::getItemData(BaseHaveItem* self,int index) {
    return &self->item_[index]; 
}

status::BaseHaveItem::BaseHaveItem(ItemData* itemData, int itemMax)
    : item_(itemData), itemMax_(itemMax) {
    for (int i = 0; i < itemMax_; ++i)
        item_[i] = ItemData();
}

void status::BaseHaveItem::clear(BaseHaveItem* self) {
    if (self->item_ && self->itemMax_ >= 1)
        for (int i = 0; i < self->itemMax_; ++i)
            self->item_[i] = ItemData();
}

void status::BaseHaveItem::setSpell(BaseHaveItem* self, int index, bool flag) {
    if (status::UseItem::isCurse(self->item_[index].index_)) {
        if (flag)
            self->item_[index].flag_.set(1);
        else
            self->item_[index].flag_.clear(1);
    }
}

bool status::BaseHaveItem::isSpell(BaseHaveItem* self,int index){
    return self->item_[index].flag_.test(1);
}


int status::BaseHaveItem::addOne(BaseHaveItem* self,int itemIndex) {
    if (itemIndex == 0)
        return -1;

    if (self->itemMax_ < 1 || self->item_ == nullptr)
        return 0;

    for (int i = 0; i < self->itemMax_; ++i) {
        uint16_t* pIndex = reinterpret_cast<uint16_t*>(&self->item_[i]);
        uint8_t* pCount = reinterpret_cast<uint8_t*>(pIndex + 1); 

        if (*pIndex == 0) {
            *pIndex = static_cast<uint16_t>(itemIndex);
            *pCount = 1;
            return i;
        }
    }

    return 0;
}

