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

    struct HaveStatusInfo {
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
        static void resetEquipment(HaveStatusInfo_0* self, int index);
        static int getDaughterCharaIndex();
        static int getSonCharaIndex();
        static int getDaughterIconIndex();
        static int getSonIconIndex();
        static void setNearDeath(HaveStatusInfo_0* self);
        static void recovery(HaveStatusInfo_0* self);
        static void revival(HaveStatusInfo_0* self);
        static void addHp(HaveStatusInfo_0* self, int hp);
        static uint16_t getStrength(HaveStatusInfo_0* self, int effect);
        static uint8_t getAgility(HaveStatusInfo_0* self, int effect);
        static uint16_t getProtection(HaveStatusInfo_0* self, int effect);
        static uint8_t getWisdom(HaveStatusInfo_0* self, int effect);
        static uint8_t getLuck(HaveStatusInfo_0* self, int effect);
        static void clearAllItem(HaveStatusInfo_0* self);
        static bool isActionEnable(HaveStatusInfo_0* self);
        static bool isActionEnableWithoutDragoram(HaveStatusInfo_0* self);
        static void setup(HaveStatusInfo_0* self, int index, bool flag);
        static void setAttack(HaveStatusInfo_0* self, uint16_t at);
        static void setDefence(HaveStatusInfo_0* self, uint16_t def);
        static void setSplit(HaveStatusInfo_0* self, bool flag);
        static bool isSplit(HaveStatusInfo_0* self);
        static void setJouk(HaveStatusInfo_0* self, bool flag);
        static bool isJouk(HaveStatusInfo_0* self);
        static void setKaishin(HaveStatusInfo_0* self, bool flag);
        static void setTsukon1(HaveStatusInfo_0* self, bool flag);
        static void setTsukon2(HaveStatusInfo_0* self, bool flag);
        static void setMiss(HaveStatusInfo_0* self, bool flag);
        static bool isMiss(HaveStatusInfo_0* self);
        static void setSleepTurn(HaveStatusInfo_0* self, bool flag);
        static bool isSleepTurn(HaveStatusInfo_0* self);
        static void setCurse2(HaveStatusInfo_0* self, bool flag);
        static bool isCurse2(HaveStatusInfo_0* self);
        static void setLifeStoneEnable(HaveStatusInfo_0* self, bool flag);
        static bool isLifeStoneEnable(HaveStatusInfo_0* self);
        static void setManusaAlready(HaveStatusInfo_0* self, bool flag);
        static bool isManusaAlready(HaveStatusInfo_0* self);
        static void setCurse1(HaveStatusInfo_0* self, bool flag);
        static bool isCurse1(HaveStatusInfo_0* self);
        static bool isDeathFlag(HaveStatusInfo_0* self);
        static bool isImmediateDeathItem(HaveStatusInfo_0* self);
        static void setCommandRandom(HaveStatusInfo_0* self, bool flag);
        static bool isCommandRandom(HaveStatusInfo_0* self);
        static void setZaoraruFailed(HaveStatusInfo_0* self, bool flag);
        static bool isZaoraruFailed(HaveStatusInfo_0* self);
        static void setMagicDisable(HaveStatusInfo_0* self, bool flag);
        static bool isMagicDisable(HaveStatusInfo_0* self);
        static void setMosyasStart(HaveStatusInfo_0* self, bool flag);
        static bool isMosyasStart(HaveStatusInfo_0* self);
        static void setAllKaishin(HaveStatusInfo_0* self, bool flag);
        static bool isAllKaishin(HaveStatusInfo_0* self);
        static void setCloseDoor(HaveStatusInfo_0* self, bool flag);
        static bool isCloseDoor(HaveStatusInfo_0* self);
        static void setMosyasTarget(HaveStatusInfo_0* self, bool flag);
        static bool isMosyasTarget(HaveStatusInfo_0* self);
        static void setDeathLessly(HaveStatusInfo_0* self, bool flag);
        static void setMosyasExec(HaveStatusInfo_0* self, bool flag);
        static bool isMosyasExec(HaveStatusInfo_0* self);
        static bool isInvokeMeganteRing(HaveStatusInfo_0* self);
        static bool isInvokeRebirthStone(HaveStatusInfo_0* self);
        static void setRebirthForRebirthStone(HaveStatusInfo_0* self, bool flag);
        static bool isRebirthForRebirthStone(HaveStatusInfo_0* self);
        static void setChangeTactics(HaveStatusInfo_0* self, bool flag);
        static bool isChangeTactics(HaveStatusInfo_0* self);
        static void setEscapeFlag(HaveStatusInfo_0* self, bool flag);
        static bool isEscapeFlag(HaveStatusInfo_0* self);
        static bool isMeganteRing(HaveStatusInfo_0* self);
        static int globalFlag_;
        static int isGlbMeganteRing();
        static void setGlbMeganteRing(bool flag);
        static void setGlbMegazaruRing(bool flag);
        static int isGlbMegazaruRing();
        static void setGlbRebirthStone(bool flag);
        static int isGlbRebirthStone();
        static void setDrawCtrlId(HaveStatusInfo_0* self, int ctrl);
        static void releaseMosyas(HaveStatusInfo_0* self);
        static void execStartOfBattle(HaveStatusInfo_0* self);
        static bool isActionEnableForMacro(HaveStatusInfo_0* self);
        static bool isAttackEnable(HaveStatusInfo_0* self);
        static void setupStatus(HaveStatusInfo_0* self, int index, bool flag);
        HaveStatusInfo();
        

        /*void clearStatusChangeInBattle();
        void setStatusChangeInBattle(DiffStatus status);*/ // ✅ retiré le `static`
    };

}

