#pragma once
#include <cstdint>
#include "status/ItemData.h"



namespace status {

    struct BaseHaveItem {
        ~BaseHaveItem();
        BaseHaveItem();
        BaseHaveItem& operator=(const BaseHaveItem& rhs);
        ItemData* item_;      // 0x08 (8 bytes on x64)
        int itemMax_;         // 0x10 (4 bytes)

        static int addNum(BaseHaveItem* self, int itemIndex, int count);
        static int getMaxCount(const BaseHaveItem* self);
        static int getItem(const BaseHaveItem* self, int index);
        static int getItemCount(const BaseHaveItem* self, int index);
        static int getCount(const BaseHaveItem* self);
        static bool isItem(const BaseHaveItem* self, int itemIndex);
        static void sort(BaseHaveItem* self);
        virtual int VBaseHaveItemAdd(int itemIndex);
        virtual bool VBaseHaveItemDel(int ctrlId);
        virtual void VBaseHaveItemSortEquipment(BaseHaveItem* self);
        static bool delOne(BaseHaveItem* self, int ctrlId);
        static bool isEquipment(const BaseHaveItem* self, int index);
        static void resetEquipment(BaseHaveItem* self, int index);
        static int getItemSortIndex(const BaseHaveItem* self, int itemIndex);
        static void resetEquipmentWithItemIndex(BaseHaveItem* self, int itemIndex);
        static void setEquipment(BaseHaveItem* self, int index);
        static int getItemMax(BaseHaveItem* self);
        static ItemData* getItemData(BaseHaveItem* self,int index);
        BaseHaveItem(ItemData* itemData, int itemMax);
        virtual void VBaseHaveItemClear();
        static void setSpell(BaseHaveItem* self,int index, bool flag);
        static bool isSpell(BaseHaveItem* self,int index);
        static int addOne(BaseHaveItem* self, int itemIndex);
        static void execThrow(int index, BaseHaveItem* haveItem);
        static bool delNum(BaseHaveItem* self, int ctrlId);
        
    };

    
} // namespace status

