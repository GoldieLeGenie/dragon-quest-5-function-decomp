#pragma once
#include <cstdint>
#include "status/BaseHaveItem.h"
#include "status/UseActionParam.h"
#include "status/BaseItem.h"
#include "dq5/ItemData.h"
#include "dq5/ItemType.h"



extern int g_Lang;

namespace status {
    namespace UseItem {

        enum SellType : int32_t {
            SELL_OK = 0x0,
            SELL_NG = 0x1,
            SELL_DIFFICULT = 0x2,
            SELL_MAX = 0x3,
        };

        enum ThrowType : int32_t
        {
            THROW_OK = 0x0,
            THROW_NG = 0x1,
            THROW_DIFFICULT = 0x2,
            THROW_MAX = 0x3,
        };


        void give(BaseHaveItem* srcHaveItem, int index, BaseHaveItem* desHaveItem);
        bool isCurse(int itemIndex);
        bool isOrder(int itemIndex);
        bool isEquipEnable(int equipAttrIndex, int itemIndex, int sex);
        bool isLost(int itemIndex);
        bool isBattleUse(int itemIndex);
        bool isUsuallyUse(int itemIndex);
        int isResult(int itemIndex);
        int getEquipValue(int itemIndex);
        dq5::level::ItemType getEquipItemType(int itemIndex); 
        dq5::level::ItemType getItemType(int itemIndex);
        int getEquipType(int itemIndex);
        int getCasino(int itemIndex);
        int getBattleUseAction(int itemIndex);
        int getBuyPrice(int itemIndex);
        int getCommentKindIndex(int itemIndex);
        int getEffect(int itemIndex);
        int getCommentType(int itemIndex);
        int getCommentMessage(int itemIndex);
        int getJudgeMessage(int itemIndex, int index);
        int getMenuMessage(int itemIndex);
        int getNameSortIndex(int itemIndex);
        BaseItem* getItem(int itemIndex);
        int getSellPrice(int itemIndex);
        SellType getSellType(int itemIndex);
        int getStopMessage(int itemIndex);
        ThrowType getThrowType(int itemIndex);
        int getTypeSortIndex(int itemIndex);
        int getUseAction(int itemIndex);
        void getUseArea(int itemIndex);









        static const short en_tbl[275] = {
        };
        static const short ko_tbl[275] = {
        };
        
        extern status::ExtraItemDokukesisou dokukesisou;

        extern uintptr_t itemData2_;
        extern bool allEquipment_;
        extern int itemIndex_;
        extern int actionIndex_;
        extern int setActionIndex;
    }
}