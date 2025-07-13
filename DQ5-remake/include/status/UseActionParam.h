#pragma once
#include <cstdint>
#include "status/CharacterStatus.h"
#include "status/HaveItem.h"
#include "status/UseActionMessage.h"
#include "status/UseActionFlag.h"
#include "status/HaveItemSack.h"


namespace status {

    struct UseActionParam {
        CharacterStatus* actorCharacterStatus_;
        CharacterStatus* targetCharacterStatus_[18];
        bool targetResult_[18];
        HaveItemSack* actorHaveItemSack_;
        int32_t actionIndex_;
        int32_t addActionIndex_;
        int32_t preActionIndex_;
        int32_t itemIndex_;
        int32_t itemSortIndex_;
        int32_t targetCount_;
        int32_t damage_;
        int32_t currentTargetIndex_;
        int32_t actionDefenceValue_;
        int32_t effectValue_;
        int32_t playerEffectValue_;
        int32_t monsterEffectValue_;
        UseActionMessage message_[18];
        UseActionFlag actionFlag_;
        bool result_;
        bool exec_;

        static void setActionIndex(UseActionParam* self, int actionIndex);
        static void setPlayerEffectValue(UseActionParam* self, int value);
        static void setMonsterEffectValue(UseActionParam* self, int value);
        static void clear(UseActionParam* self);
        static void clearMessage(UseActionParam* self);
        UseActionParam();
    };

}

