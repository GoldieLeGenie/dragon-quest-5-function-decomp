#include "status/BaseHaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"
#include "args/ExcelBinaryData.h"
#include "dq5/ItemData.h"
#include "dq5/AppriseItem.h"
#include "status/BaseItem.h"
#include "status/UseActionParam.h"
#include "status/BaseItem.h"
#include "status/UseAction.h"
#include <cstdint>

uintptr_t status::UseItem::itemData2_;

bool status::UseItem::allEquipment_; 
int status::UseItem::itemIndex_;
int status::UseItem::actionIndex_;
int g_Lang = 0;

void status::UseItem::give(status::BaseHaveItem* srcHaveItem, int index, status::BaseHaveItem* desHaveItem)
{
    int Count; 
    int v7; 
    int Item; 
    int v9; 
    int v10; 
    int v11; 

    Count = status::BaseHaveItem::getCount(desHaveItem);
    if (Count == status::BaseHaveItem::getItemMax(desHaveItem))
    {
        if (status::BaseHaveItem::getItemMax(desHaveItem) != 99)
        {
            v7 = status::BaseHaveItem::getCount(desHaveItem);
            Item = status::BaseHaveItem::getItem(desHaveItem, v7 - 1);
            v9 = status::BaseHaveItem::getCount(desHaveItem);
            (*((void(__fastcall**)(status::BaseHaveItem*, int))desHaveItem->_vptr$BaseHaveItem + 2))(desHaveItem, v9 - 1);
            v10 = status::BaseHaveItem::getItem(srcHaveItem, index);
            (*((void(__fastcall**)(status::BaseHaveItem*, int))desHaveItem->_vptr$BaseHaveItem + 1))(desHaveItem, v10);
            (*((void(__fastcall**)(status::BaseHaveItem*, int))srcHaveItem->_vptr$BaseHaveItem + 2))(srcHaveItem, index);
            (*((void(__fastcall**)(status::BaseHaveItem*, int))srcHaveItem->_vptr$BaseHaveItem + 1))(srcHaveItem, Item);
        }
    }
    else
    {
        v11 = status::BaseHaveItem::getItem(srcHaveItem, index);
        (*((void(__fastcall**)(status::BaseHaveItem*, int))desHaveItem->_vptr$BaseHaveItem + 1))(desHaveItem, v11);
        (*((void(__fastcall**)(status::BaseHaveItem*, int))srcHaveItem->_vptr$BaseHaveItem + 2))(srcHaveItem, index);
    }
}


bool status::UseItem::isCurse(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    if (!record) return false;

    itemData2_ = reinterpret_cast<uintptr_t>(record);
    return ((*reinterpret_cast<uint8_t*>(itemData2_ + 39)) >> 2) & 1;
}

bool status::UseItem::isOrder(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    if (!record) return false;

    itemData2_ = reinterpret_cast<uintptr_t>(record);
    return ((*reinterpret_cast<uint8_t*>(itemData2_ + 40)) & 0x03) != 0;
}

int status::UseItem::isResult(int itemIndex) {
    return 1;
}

bool status::UseItem::isEquipEnable(int equipAttrIndex, int itemIndex, int sex) {
    if (!status::UseItem::allEquipment_) {
        void* record = dq5::level::ItemData::binary_.getRecord(
            itemIndex,
            dq5::level::ItemData::addr_,
            dq5::level::ItemData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
        );
        if (!record) return false;

        itemData2_ = reinterpret_cast<uintptr_t>(record);
        uint8_t* data = reinterpret_cast<uint8_t*>(record);

        int byteOffset = 36;
        if (equipAttrIndex >= 9 && equipAttrIndex <= 16) ++byteOffset;
        else if (equipAttrIndex >= 17 && equipAttrIndex <= 24) byteOffset += 2;
        if (equipAttrIndex == 25) byteOffset = 39;

        uint8_t value = data[byteOffset];
        switch (equipAttrIndex % 8) {
        case 1: return value & 0x01;
        case 2: return value & 0x02;
        case 3: return value & 0x04;
        case 4: return value & 0x08;
        case 5: return value & 0x10;
        case 6: return value & 0x20;
        case 7: return value & 0x40;
        case 0: return static_cast<int8_t>(value) < 0;
        default: return false;
        }
    }

    return true;
}

bool status::UseItem::isLost(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    if (!record) return false;

    itemData2_ = reinterpret_cast<uintptr_t>(record);
    return ((*reinterpret_cast<uint8_t*>(itemData2_ + 39)) >> 1) & 1;
}


bool status::UseItem::isBattleUse(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    if (!record) return false;

    itemData2_ = reinterpret_cast<uintptr_t>(record);
    uint16_t value = *reinterpret_cast<uint16_t*>(itemData2_ + 26);
    if (value != 219) {
        record = dq5::level::ItemData::binary_.getRecord(
            itemIndex,
            dq5::level::ItemData::addr_,
            dq5::level::ItemData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
        );
        itemData2_ = reinterpret_cast<uintptr_t>(record);
        return (*reinterpret_cast<uint16_t*>(itemData2_ + 26)) != 220;
    }

    return false;
}


bool status::UseItem::isUsuallyUse(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    itemData2_ = reinterpret_cast<uintptr_t>(record);
    return (*reinterpret_cast<uint16_t*>(itemData2_ + 24)) != 281;
}

int status::UseItem::getEquipValue(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    return *reinterpret_cast<const uint16_t*>(static_cast<uint8_t*>(record) + 22);
}


int status::UseItem::getEquipType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    return 0;
}


int status::UseItem::getCasino(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    
    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<const int*>(record);  
}


int status::UseItem::getBattleUseAction(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

 
    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<const uint16_t*>(reinterpret_cast<uint8_t*>(record) + 26);  
}


int status::UseItem::getBuyPrice(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );


    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<const uint16_t*>(reinterpret_cast<uint8_t*>(record) + 16);
}


int status::UseItem::getCommentKindIndex(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<const uint8_t*>(reinterpret_cast<uint8_t*>(record) + 33);
}

int status::UseItem::getCommentMessage(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<const uint8_t*>(reinterpret_cast<uint8_t*>(record) + 12);
}

int status::UseItem::getCommentType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<const uint8_t*>(reinterpret_cast<uint8_t*>(record) + 34);
}

int status::UseItem::getEffect(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<const uint16_t*>(reinterpret_cast<uint8_t*>(record) + 22);
}



status::BaseItem* status::UseItem::getItem(int itemIndex) {
    if (itemIndex == 112) {
        static status::ExtraItemDokukesisou dokukesisou;
        dokukesisou.available_ = 0;
        return &dokukesisou;
    }
    else if (itemIndex == 111) {
        static status::ExtraItemYakusou yakusou;  
        yakusou.available_ = 0;
        return &yakusou;
    }
    else {
        static status::ExtraItemDokukesisou item;
        item.available_ = 0;
        return &item;
    }
}

dq5::level::ItemType status::UseItem::getItemType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return static_cast<dq5::level::ItemType>(9);

    // Lecture du type brut à l'offset 32 (byte)
    uint8_t raw = *(reinterpret_cast<uint8_t*>(record) + 35);

    if (raw == 0 || raw > 11)
        return static_cast<dq5::level::ItemType>(9);  // fallback si hors limites

    // Table réelle d'après .rodata:dword_258340
    static const uint8_t typeMap[11] = {
        0,  // 1 → 0
        1,  // 2 → 1
        3,  // 3 → 3
        2,  // 4 → 2
        4,  // 5 → 4
        5,  // 6 → 5
        7,  // 7 → 7
        11, // 8 → 0xB
        10, // 9 → 0xA
        9,  // 10 → 9
        12  // 11 → 0xC
    };

    return static_cast<dq5::level::ItemType>(typeMap[raw - 1]);
}



dq5::level::ItemType status::UseItem::getEquipItemType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return static_cast<dq5::level::ItemType>(9);

    // Lecture du type brut à l'offset 32 (byte)
    uint8_t raw = *(reinterpret_cast<uint8_t*>(record) + 32);

    if (raw == 0 || raw > 11)
        return static_cast<dq5::level::ItemType>(9);  // fallback si hors limites

    // Table réelle d'après .rodata:dword_258340
    static const uint8_t typeMap[11] = {
        0,  // 1 → 0
        1,  // 2 → 1
        3,  // 3 → 3
        2,  // 4 → 2
        4,  // 5 → 4
        5,  // 6 → 5
        7,  // 7 → 7
        11, // 8 → 0xB
        10, // 9 → 0xA
        9,  // 10 → 9
        12  // 11 → 0xC
    };

    return static_cast<dq5::level::ItemType>(typeMap[raw - 1]);
}


int status::UseItem::getJudgeMessage(int itemIndex, int index) {
    if (index < 0 || index > 8)
        return 0;

    // Récupère le record depuis AppriseItem
    void* record = dq5::level::AppriseItem::binary_.getRecord(
        itemIndex,
        dq5::level::AppriseItem::addr_,
        dq5::level::AppriseItem::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::AppriseItem::loadSwitch_)
    );

    if (!record)
        return 0;

    int* data = static_cast<int*>(record);

    return data[index + 1];  
}



int status::UseItem::getMenuMessage(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<int*>(status::UseItem::itemData2_ + 4);
}



int status::UseItem::getNameSortIndex(int itemIndex) {
    if (g_Lang == 2) {
        return ko_tbl[itemIndex];
    }
    else if (g_Lang == 1) {
        return en_tbl[itemIndex];
    }

    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    return *reinterpret_cast<uint16_t*>(status::UseItem::itemData2_ + 30);
}


int status::UseItem::getSellPrice(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<uint16_t*>(status::UseItem::itemData2_ + 18);
}



status::UseItem::SellType status::UseItem::getSellType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return status::UseItem::SELL_NG;  

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    uint8_t field = *reinterpret_cast<uint8_t*>(status::UseItem::itemData2_ + 39);
    uint8_t bits = (field >> 4) & 0x3;

    if (bits == 2)
        return status::UseItem::SELL_DIFFICULT;

    return (bits != 1) ? status::UseItem::SELL_OK : status::UseItem::SELL_NG;
}


int status::UseItem::getStopMessage(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<int*>(status::UseItem::itemData2_ + 8);
}


status::UseItem::ThrowType status::UseItem::getThrowType(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return status::UseItem::THROW_OK;  // valeur par défaut de repli

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    uint8_t raw = *reinterpret_cast<uint8_t*>(status::UseItem::itemData2_ + 39);
    uint8_t type = (raw >> 6) & 0x3;  // extrait les bits 6-7 (2 bits)

    switch (type) {
    case 0: return status::UseItem::THROW_OK;
    case 1: return status::UseItem::THROW_NG;
    case 2: return status::UseItem::THROW_DIFFICULT;
    default: return status::UseItem::THROW_OK; 
    }
}


int status::UseItem::getTypeSortIndex(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<uint16_t*>(status::UseItem::itemData2_ + 28);
}



int status::UseItem::getUseAction(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return 0;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    return *reinterpret_cast<uint16_t*>(status::UseItem::itemData2_ + 24);
}



void status::UseItem::getUseArea(int itemIndex) {
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    if (!record)
        return;

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    uint16_t actionId = *reinterpret_cast<uint16_t*>(itemData2_ + 24);
    status::UseAction::getUseArea(actionId);
}



















