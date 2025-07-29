#include "status/HaveItemSack.h"
#include "status/HaveItem.h"
#include "status/BaseHaveItem.h"
#include "status/UseItem.h"
#include "iostream"
#include <vector>

void status::HaveItemSack::execThrow(status::HaveItemSack* self, int index)
{
    self->VBaseHaveItemDel(index);
}

void status::HaveItemSack::fill(status::HaveItemSack* self)
{
    for (int i = 1; i <= 273; ++i)
    {
        if (i != 256)
            self->VBaseHaveItemAdd(i);
    }
}


int status::HaveItemSack::add(HaveItemSack* self, int itemIndex, int count)
{

    int ItemSortIndex = status::BaseHaveItem::getItemSortIndex(self, itemIndex);
    if (status::BaseHaveItem::getItemCount(self, ItemSortIndex) + count >= 100)
        count = 99 - status::BaseHaveItem::getItemCount(self, ItemSortIndex);
    return status::BaseHaveItem::addNum(self, itemIndex, count);
}


bool status::HaveItemSack::del(BaseHaveItem* self, int ctrlId)
{
    return status::BaseHaveItem::delNum(self, ctrlId);
}


void status::HaveItemSack::sortOutItemOne(HaveItemSack* self, status::HaveItem* haveItem)
{
    int count = status::BaseHaveItem::getCount(haveItem);

    if (count < 1)
        return;

    for (int i = 0; i < count; ++i)
    {
        if (!status::BaseHaveItem::isEquipment(haveItem, i))
        {
            int item = status::BaseHaveItem::getItem(haveItem, i);
            if (status::UseItem::isOrder(item))
            {
                self->VBaseHaveItemAdd(item);         
                haveItem->VBaseHaveItemDel(i);
                return;
            }
        }
    }
}

int status::HaveItemSack::add(HaveItemSack* self, int itemIndex)
{
    return status::HaveItemSack::add(self, itemIndex, 1);
}

void status::HaveItemSack::sortOutItem(HaveItemSack* self, status::HaveItem* haveItem)
{
    int maxCount = status::BaseHaveItem::getMaxCount(haveItem);
    for (int i = 0; i < maxCount; ++i)
    {
        status::HaveItemSack::sortOutItemOne(self, haveItem);
    }
}


void status::HaveItemSack::sortOutSack(HaveItemSack* self, SortType type) {
    SortType sortType = (type == SortType::None) ? self->sortType_ : type;
    self->sortType_ = sortType;

    std::vector<status::ItemData> sortedItems;
    sortedItems.reserve(self->itemMax_);

    for (int i = 0; i < self->itemMax_; ++i) {
        sortedItems.emplace_back(); 
    }

    for (int sortIndex = 1; sortIndex <= self->itemMax_; ++sortIndex) {
        for (int i = 0; i < self->itemMax_; ++i) {
            int index = self->item_[i].index_;
            if (index == 0)
                continue;

            int currentSortIndex = 0;
            switch (sortType) {
            case SortType::ByName:
                currentSortIndex = status::UseItem::getNameSortIndex(index);
                break;
            case SortType::ByType:
                currentSortIndex = status::UseItem::getTypeSortIndex(index);
                break;
            default:
                continue;
            }

            if (sortIndex == currentSortIndex) {
                sortedItems.push_back(self->item_[i]);
                self->item_[i].index_ = 0; 
            }
        }
    }

    int j = 0;
    for (const auto& item : sortedItems) {
        if (item.index_ != 0 && j < self->itemMax_) {
            self->item_[j++] = item;
        }
    }
}


void status::HaveItemSack::setNameString(HaveItemSack* self, const char* name)
{
    strcpy(self->name_, name);
}


void status::HaveItemSack::operator=(const status::HaveItemSack* rhs)
{
    int Count; // r0
    int CurrentCount; // r6
    int i; // r7
    int Item; // r5
    int ItemCount; // r0

    Count = status::BaseHaveItem::getCount(rhs);
    if (Count >= 1)
    {
        CurrentCount = Count;
        for (i = 0; i != CurrentCount; ++i)
        {
            Item = status::BaseHaveItem::getItem(rhs, i);
            ItemCount = status::BaseHaveItem::getItemCount(rhs, i);
            status::HaveItemSack::add(this, Item, ItemCount);
        }
    }
}