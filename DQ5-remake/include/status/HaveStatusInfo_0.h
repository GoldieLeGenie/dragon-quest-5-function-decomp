// HaveStatusInfo_0.h
#pragma once
#include "ar/BitFlag.h"
#include "status/ActionDefence.h"
#include "status/HaveBattleStatus.h"
#include "status/HaveStatus.h"
#include "status/HaveItem.h"
#include "status/HaveAction.h"
#include "status/HaveEquipment.h"
#include "status/StatusChange.h"
#include "dq5/CommandTypeDebug.h"
#include "dq5/CharacterType.h"

namespace status {
    struct HaveBattleStatus;
    struct HaveStatusInfo;

    struct HaveStatusInfo_0 {
        int hp_[4];                          // 0x00
        int mp_[4];                          // 0x10
        ar::Flag32 status_[8];              // 0x20
        dq5::level::CharacterType characterType_;  // 0x40
        int index_;                          // 0x44
        HaveBattleStatus* haveBattleStatus_; // 0x48
        ActionDefence actionDefence_;       // 0x4C (size 0x2C)
        int effectValue_;                   // 0x78
        int effectValueMax_;                // 0x7C
        int effectValueMin_;                // 0x80
        int effectValueCount_;              // 0x84
        int specialTargetDamage_;           // 0x88
        int specialTargetCount_;            // 0x8C
        int addDamage_;                     // 0x90
        int drawCtrlId_;                    // 0x94
        bool noDamage_;                     // 0x98
        bool allEquipment_;                 // 0x99
        bool rebirthFlag_;                  // 0x9A

        int16_t defenceChange_;             // 0x9C
        int16_t agilityChange_;             // 0x9E
        int16_t attackChange_;              // 0xA0

        ar::Flag32 flag_;                   // 0xA4
        ar::Flag32 flag2_;                  // 0xA8
        ar::Flag32 testFlag_;               // 0xAC
        ar::Flag32 battleFlag_;             // 0xB0

        HaveStatus haveStatus_;             // 0xB4
        HaveItem haveItem_;                 // 0xEC
        HaveAction haveAction_;             // 0x128
        HaveEquipment haveEquipment_;       // 0x19C
        StatusChange statusChange_;         // 0x1BC
        dq5::level::CommandTypeDebug commandTypeDebug_; // 0x420

        
    };

} // namespace status
