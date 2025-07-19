#include "status/BaseHaveItem.h"
#include "status/UseItem.h"
#include "dq5/ItemType.h"
#include "args/ExcelBinaryData.h"
#include "dq5/ItemData.h"
#include "dq5/AppriseItem.h"
#include "status/BaseItem.h"
#include "status/UseActionParam.h"
#include "status/UseAction.h"
#include <cstdint>
#include "iostream"

uintptr_t status::UseItem::itemData2_;

bool status::UseItem::allEquipment_; 
int status::UseItem::itemIndex_;
int status::UseItem::actionIndex_;
int g_Lang = 0;

void status::UseItem::give(status::BaseHaveItem* srcHaveItem, int index, status::BaseHaveItem* desHaveItem)
{
    int desCount = status::BaseHaveItem::getCount(desHaveItem);
    int desMax = status::BaseHaveItem::getItemMax(desHaveItem);

    if (desCount == desMax)
    {
        // Cas spécial : inventaire plein mais max < 99
        if (desMax != 99)
        {
            int lastIndex = desCount - 1;
            int lastItem = status::BaseHaveItem::getItem(desHaveItem, lastIndex);

            // Supprimer le dernier objet de la destination
            status::BaseHaveItem::del(desHaveItem, lastIndex);

            // Ajouter l'objet source dans la destination
            int itemToMove = status::BaseHaveItem::getItem(srcHaveItem, index);
            status::BaseHaveItem::add(desHaveItem, itemToMove);

            // Supprimer l’objet source de son inventaire
            status::BaseHaveItem::del(srcHaveItem, index);

            // Réinsérer l’ancien dernier objet dans la source
            status::BaseHaveItem::add(srcHaveItem, lastItem);
        }
    }
    else
    {
        // Transfert direct si la destination n’est pas pleine
        int itemToMove = status::BaseHaveItem::getItem(srcHaveItem, index);
        status::BaseHaveItem::add(desHaveItem, itemToMove);
        status::BaseHaveItem::del(srcHaveItem, index);
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


void status::UseItem::getUseType(int itemIndex)
{
    // Récupérer le pointeur vers l'enregistrement ItemData
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemIndex,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
    uint16_t useType = *reinterpret_cast<const uint16_t*>(
        static_cast<const char*>(record) + 24
        );
    status::UseAction::getUseType(useType);
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




void status::UseItem::execUse(UseActionParam* useActionParam)
{
    auto* actorSack = useActionParam->actorHaveItemSack_;
    int sortIndex = useActionParam->itemSortIndex_;
    int itemId = 0;

    if (actorSack) {
        itemId = status::BaseHaveItem::getItem(actorSack, sortIndex);
        status::UseItem::itemIndex_ = itemId;
    }
    else if (auto* actor = useActionParam->actorCharacterStatus_) {
        itemId = status::BaseHaveItem::getItem(&actor->haveStatusInfo_.haveItem_, sortIndex);
        status::UseItem::itemIndex_ = itemId;
        useActionParam->itemIndex_ = itemId;
    }
    else {
        itemId = status::UseItem::itemIndex_;
    }

    // Récupération des données d'objet depuis les données binaires
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemId,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    // Récupère l'index de l'action de l'objet
    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    int actionIndex = *reinterpret_cast<uint16_t*>(status::UseItem::itemData2_ + 24);
    status::UseItem::actionIndex_ = actionIndex;
    useActionParam->setActionIndex(useActionParam, actionIndex);

    // Exécute l'action
    status::UseAction::execUse(useActionParam);

    // Si l'action a été exécutée avec succès
    if (useActionParam->result_) {
        record = dq5::level::ItemData::binary_.getRecord(
            status::UseItem::itemIndex_,
            dq5::level::ItemData::addr_,
            dq5::level::ItemData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
        );

        if (*reinterpret_cast<uint8_t*>(status::UseItem::itemData2_ + 39) & 0x2) {
            if (actorSack) {
                status::UseItem::itemIndex_ = status::BaseHaveItem::del(actorSack, sortIndex);
            }
            else if (auto* actor = useActionParam->actorCharacterStatus_) {
                status::BaseHaveItem::del(&actor->haveStatusInfo_.haveItem_, sortIndex);
            }
        }

        // Si l'anneau de prière est brisé, on le retire aussi
        if (status::UseActionFlag::isBreakPrayRing()) {
            status::UseActionFlag::setBreakPrayRing(false);

            if (actorSack) {
                status::UseItem::itemIndex_ = status::BaseHaveItem::del(actorSack, sortIndex);
            }
            else if (auto* actor = useActionParam->actorCharacterStatus_) {
                status::BaseHaveItem::del(&actor->haveStatusInfo_.haveItem_, sortIndex);
            }
        }
    }
}


void status::UseItem::execThrow(int index, BaseHaveItem* haveItem)
{
    status::BaseHaveItem::del(haveItem,index);
}



int status::UseItem::getJudgeMessageCount(int itemIndex)
{
    void* record = dq5::level::AppriseItem::binary_.getRecord(
        itemIndex,
        dq5::level::AppriseItem::addr_,
        dq5::level::AppriseItem::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::AppriseItem::loadSwitch_)
    );

    if (!record)
        return 0;

    int* data = static_cast<int*>(record);

    int count = 1;  // v7

    int v2 = data[1];  // champ 1
    int v3 = data[2];  // champ 2
    int v4 = data[3];  // champ 3
    int v5 = data[4];  // champ 4
    int v6 = data[5];  // champ 5
    int v8 = data[6];  // champ 6
    int v9 = data[7];  // champ 7

    // Si le premier champ (data[1]) est non nul, on commence à 2
    if (v2)
        count = 2;

    // Si data[2] est non nul, le count ne change pas (on garde count)
    if (v3)
        count = count;

    // Si data[3] à data[7] sont non nuls, on incrémente
    if (v4) ++count;
    if (v5) ++count;
    if (v6) ++count;
    if (v8) ++count;
    if (v9) ++count;

    return count;
}

void status::UseItem::give2(HaveStatusInfo_0* srcHaveStatusInfo,int srcIndex,HaveStatusInfo_0* desHaveStatusInfo,int desIndex)
{
    status::HaveItem* srcHaveItem = &srcHaveStatusInfo->haveItem_;
    bool isEquipment = status::BaseHaveItem::isEquipment(srcHaveItem, srcIndex);

    if (desIndex == -1)
    {
        int itemId = 0;
        if (isEquipment)
            itemId = status::BaseHaveItem::getItem(srcHaveItem, srcIndex);

        status::UseItem::give(srcHaveItem, srcIndex, &desHaveStatusInfo->haveItem_);

        if (isEquipment)
            status::HaveEquipment::resetEquipment(&srcHaveStatusInfo->haveEquipment_, itemId);
    }
    else
    {
        if (isEquipment)
        {
            int itemId = status::BaseHaveItem::getItem(srcHaveItem, srcIndex);
            status::HaveEquipment::resetEquipment(&srcHaveStatusInfo->haveEquipment_, itemId);
        }

        if (status::BaseHaveItem::isEquipment(&desHaveStatusInfo->haveItem_, desIndex))
        {
            int itemId = status::BaseHaveItem::getItem(&desHaveStatusInfo->haveItem_, desIndex);
            status::HaveEquipment::resetEquipment(&desHaveStatusInfo->haveEquipment_, itemId);
        }

        status::ItemData* srcItemData = status::BaseHaveItem::getItemData(srcHaveItem, srcIndex);
        status::ItemData* desItemData = status::BaseHaveItem::getItemData(&desHaveStatusInfo->haveItem_, desIndex);

        status::ItemData::setEquipment(srcItemData + 3, false);
        status::ItemData::setEquipment(desItemData + 3, false);

        std::swap((srcItemData + 3)->index_, (desItemData + 3)->index_);
        status::HaveItem::sortEquipment(srcHaveItem);
        status::HaveItem::sortEquipment(&desHaveStatusInfo->haveItem_);
    }
}



void status::UseItem::give2(HaveItemSack* srcHaveItemSack,int srcIndex,HaveStatusInfo_0* desHaveStatusInfo,int desIndex)
{
    if (desIndex == -1) {
        status::UseItem::give(srcHaveItemSack, srcIndex, &desHaveStatusInfo->haveItem_);
    }
    else {
        if (status::BaseHaveItem::isEquipment(&desHaveStatusInfo->haveItem_, desIndex)) {
            int itemId = status::BaseHaveItem::getItem(&desHaveStatusInfo->haveItem_, desIndex);
            status::HaveEquipment::resetEquipment(&desHaveStatusInfo->haveEquipment_, itemId);
        }

        status::ItemData* srcItemData = status::BaseHaveItem::getItemData(srcHaveItemSack, srcIndex);
        status::ItemData* desItemData = reinterpret_cast<status::ItemData*>(
            reinterpret_cast<char*>(&desHaveStatusInfo->hp_[desIndex]) + sizeof(status::ItemData) * 62
            );

        int oldIndex = desItemData->index_;
        desItemData->index_ = srcItemData->index_;
        status::ItemData::setEquipment(desItemData, false);
        status::HaveItemSack::del(srcHaveItemSack, srcIndex);
        status::HaveItemSack::add(srcHaveItemSack, oldIndex);
    }
}

void status::UseItem::give2(HaveStatusInfo_0* srcHaveStatusInfo,int srcIndex,HaveItemSack* desHaveItemSack,int desIndex)
{
    status::HaveItem* srcHaveItem = &srcHaveStatusInfo->haveItem_;
    bool isEquipment = status::BaseHaveItem::isEquipment(srcHaveItem, srcIndex);

    if (desIndex == -1) {
        int itemId = 0;
        if (isEquipment)
            itemId = status::BaseHaveItem::getItem(srcHaveItem, srcIndex);

        status::UseItem::give(srcHaveItem, srcIndex, desHaveItemSack);

        if (isEquipment)
            status::HaveEquipment::resetEquipment(&srcHaveStatusInfo->haveEquipment_, itemId);
    }
    else {
        if (isEquipment)
            status::HaveStatusInfo::resetEquipment(srcHaveStatusInfo, srcIndex);

        // Lire l’item ID dans le sac destination
        uint8_t itemId = status::BaseHaveItem::getItem(desHaveItemSack, desIndex);

        // Accès à l’entrée HP du personnage source
        uint16_t& statusItemIndex = reinterpret_cast<uint16_t*>(
            &srcHaveStatusInfo->hp_[srcIndex]
            )[124];  // offset 124 → champ index_

        // add l’ancien item dans le sac
        status::HaveItemSack::add(desHaveItemSack, statusItemIndex);

        // remove l’objet à desIndex
        status::HaveItemSack::del(desHaveItemSack, desIndex);

        // remplacer l’index dans HaveStatusInfo_0 par celui du nouvel item
        statusItemIndex = itemId;
    }
}


void status::UseItem::give(HaveStatusInfo_0* srcHaveStatusInfo,int index,HaveStatusInfo_0* desHaveStatusInfo)
{
    status::HaveItem* srcHaveItem = &srcHaveStatusInfo->haveItem_;
    int itemId = 0;

    // Si c’est un équipement, récupérer son ID pour déséquipement après
    if (status::BaseHaveItem::isEquipment(srcHaveItem, index)) {
        itemId = status::BaseHaveItem::getItem(srcHaveItem, index);
    }

    // Transfert de l’objet vers la destination
    status::UseItem::give(srcHaveItem, index, &desHaveStatusInfo->haveItem_);

    // Déséquipement si nécessaire
    if (itemId != 0) {
        status::HaveEquipment::resetEquipment(&srcHaveStatusInfo->haveEquipment_, itemId);
    }
}

void status::UseItem::execBattleUse(UseActionParam* useActionParam)
{
    int itemSortIndex = useActionParam->itemSortIndex_;

    // Récupérer l’ID de l’objet dans l’inventaire du personnage
    int itemId = status::BaseHaveItem::getItem(
        &useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_,
        itemSortIndex
    );

    // Mémoriser l’item utilisé
    status::UseItem::itemIndex_ = itemId;
    useActionParam->itemIndex_ = itemId;

    // Récupérer les données binaires de l’objet (ItemData)
    void* record = dq5::level::ItemData::binary_.getRecord(
        itemId,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    // Lire l'actionIndex à l’offset +26 (uint16_t)
    uint16_t actionIndex = *reinterpret_cast<const uint16_t*>(
        static_cast<const char*>(record) + 26
        );

    status::UseItem::actionIndex_ = actionIndex;
    status::UseActionParam::setActionIndex(useActionParam, actionIndex);

    // Exécuter l’action liée à l’objet
    status::UseAction::execUse(useActionParam);

    // Recharger le record (si modifié entre-temps)
    record = dq5::level::ItemData::binary_.getRecord(
        status::UseItem::itemIndex_,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );

    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);

    // Vérifier s’il est consommable (bit 1 à l’offset +39)
    uint8_t flag = *reinterpret_cast<const uint8_t*>(
        static_cast<const char*>(record) + 39
        );

    if (flag & 0x02)
    {
        status::BaseHaveItem::del(
            &useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_,
            itemSortIndex
        );
    }
}


void status::UseItem::setItemRecord(int index)
{
    void* record = dq5::level::ItemData::binary_.getRecord(
        index,
        dq5::level::ItemData::addr_,
        dq5::level::ItemData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ItemData::loadSwitch_)
    );
 
    status::UseItem::itemData2_ = reinterpret_cast<uintptr_t>(record);
}

void status::UseItem::initialize()
{
    // No initialization required
}
