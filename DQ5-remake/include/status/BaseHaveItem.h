#pragma once
#include <cstdint>
#include "status/ItemData.h"

#pragma pack(push, 1)

namespace status {

    struct alignas(4) BaseHaveItem {
        ~BaseHaveItem();
        BaseHaveItem& operator=(const BaseHaveItem& rhs);

        int (**_vptr$BaseHaveItem)(void);  // 0x00 (8 bytes on x64)
        ItemData* item_;      // 0x08 (8 bytes on x64)
        int itemMax_;         // 0x10 (4 bytes)

        static int addNum(BaseHaveItem* self, int itemIndex, int count);
        static int getMaxCount(const BaseHaveItem* self);
        static int getItem(const BaseHaveItem* self, int index);
        static int getItemCount(const BaseHaveItem* self, int index);
        static int getCount(const BaseHaveItem* self);
        static bool isItem(const BaseHaveItem* self, int itemIndex);
        static void sort(BaseHaveItem* self);
        static int add(BaseHaveItem* self, int itemIndex);
        static bool del(BaseHaveItem* self, int ctrlId);
        static void sortEquipment(BaseHaveItem* self);
        BaseHaveItem();
        static bool delOne(BaseHaveItem* self, int ctrlId);
        static bool isEquipment(const BaseHaveItem* self, int index);
        static void resetEquipment(BaseHaveItem* self, int index);
        static int getItemSortIndex(const BaseHaveItem* self, int itemIndex);
        static void resetEquipmentWithItemIndex(BaseHaveItem* self, int itemIndex);
        static void setEquipment(BaseHaveItem* self, int index);
        static int getItemMax(BaseHaveItem* self);
        ItemData* getItemData(BaseHaveItem* self,int index);
        BaseHaveItem(ItemData* itemData, int itemMax);
        static void clear(BaseHaveItem* self);
        static void setSpell(BaseHaveItem* self,int index, bool flag);
        static bool isSpell(BaseHaveItem* self,int index);
        static int addOne(BaseHaveItem* self, int itemIndex);
    };

    
} // namespace status

#pragma pack(pop)