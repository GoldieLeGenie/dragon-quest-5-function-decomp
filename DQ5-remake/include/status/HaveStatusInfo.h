#pragma once
#include <cstdint>
#include "ar/BitFlag.h"
#include "dq5/CharacterType.h"
#include "status/ActionDefence.h"
#include "status/HaveStatus.h"
#include "status/HaveItem.h"
#include "status/HaveAction.h"
#include "status/HaveEquipment.h"
#include "status/StatusChange.h"
#include "dq5/CommandTypeDebug.h"

namespace status {

    struct HaveBattleStatus;
    struct HaveStatusInfo_0;

    enum class DiffStatus : int32_t {
        BeforeAction = 0,
        ExecuteAction = 1,
        ResultAction = 2,
        SpecialAction = 3,
        MaxAction = 4
    };

    struct alignas(8) HaveStatusInfo {
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

        static bool isDeath(const HaveStatusInfo_0* self);
        static uint16_t getHp(const HaveStatusInfo_0* self);
        static void clearHpInBattle(HaveStatusInfo_0* self);
        static void clearMpInBattle(HaveStatusInfo_0* self);
        static int getGold(const HaveStatusInfo_0* self);
        static void clearStatusChangeInBattle(HaveStatusInfo_0* self);
        static void setStatusChangeInBattle(HaveStatusInfo_0* self, DiffStatus status);
        static void setImmediateDeathItem(status::HaveStatusInfo_0* self, bool flag);
        static uint16_t getAttack(HaveStatusInfo_0* self, int effect);
        static uint16_t getDefence(HaveStatusInfo_0* self, int effect);
        static bool isKaishin(HaveStatusInfo_0* self);
        static bool isTsukon1(HaveStatusInfo_0* self);
        static bool isTsukon2(HaveStatusInfo_0* self);
        static bool isMultiAttack(HaveStatusInfo_0* self);
        static void setMultiAttack(HaveStatusInfo_0* self, bool flag);
        static uint16_t getHpMax(const status::HaveStatusInfo_0* self);
        static void setMp(status::HaveStatusInfo_0* self, uint16_t mp);
        static uint16_t getMp(const status::HaveStatusInfo_0* self);
        static void HaveStatusinfo(HaveStatusInfo_0* self);
        static void setDeath(HaveStatusInfo_0* self);
        static void setHp(HaveStatusInfo_0* self, uint16_t hp);
        static int setItemEquipment(HaveStatusInfo_0* self, int itemIndex);
        static void setEquipment(HaveStatusInfo_0* self, int index);
        /*void clearStatusChangeInBattle();
        void setStatusChangeInBattle(DiffStatus status);*/ // ✅ retiré le `static`
    };

}
