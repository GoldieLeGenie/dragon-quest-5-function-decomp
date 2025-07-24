#pragma once

#include "status/HaveStatusInfo_0.h"
#include "status/HaveBattleStatus.h"
#include "dq5/CharacterType.h"
#include "ar/BitFlag.h"

namespace status {

    struct CharacterStatus {
   
        dq5::level::CharacterType characterType_;           // 0x04

        // Infos de statut global (objets, compétences, etc.)
        HaveStatusInfo haveStatusInfo_;              // 0x08

        // Infos spécifiques au combat
        HaveBattleStatus haveBattleStatus_;          // 0x42C

        // Identifiants divers
        int characterIndex_;                         // 0x4DC
        int characterGroup_;                         // 0x4E0
        int arrayIndex_;                             // 0x4E4

        // Flags
        ar::Flag32 menuStatusFlag_;                    // 0x4E8

        // Animations de dégâts (probablement des enums ou structs légers)
        enum class DamageAnimation : int32_t {
            NoAnim = 0x0,
            Damage = 0x1,
            Death = 0x2,
            Disappear = 0x3,
            MultiDamage = 0x4
        };

        enum class DamageSound : int32_t {
            NoSe = 0x0,
            KaishinSe = 0x1,
            TsukonSe = 0x2
        };

        DamageAnimation damageAnimation_;            // 0x4EC
        DamageAnimation damageAnimationMulti_;       // 0x4F0
        DamageSound damageSound_;                    // 0x4F4

        // Flags liés à l'animation
        ar::Flag32 damageAnimationFlag_;               // 0x4F8
        ar::Flag32 type_;                              // 0x4FC
        static void setDamageAnimation(CharacterStatus* self);
        virtual bool isDeathDisable(CharacterStatus* self);
        virtual void setMosyasRelease(CharacterStatus* self);
        virtual void setMosyasChange(CharacterStatus* self, CharacterStatus* target);
        virtual void setMosyasChange(CharacterStatus* self, int monsterIndex);
        static bool isRebirthAnimation(CharacterStatus* self);
        static void clearRebirthAnimation(CharacterStatus* self);
        static void setRebirthAnimation(CharacterStatus* self);
        static bool isEscapeAnimation(CharacterStatus* self);
        static void clearEscapeAnimation(CharacterStatus* self);
        static void setEscapeAnimation(CharacterStatus* self);
        static bool isMultiDamageAnimation2nd(CharacterStatus* self);
        static void clearMultiDamageAnimation2nd(CharacterStatus* self);
        static void setMultiDamageAnimation2nd(CharacterStatus* self);
        static bool isMultiDamageAnimation(CharacterStatus* self);
        static void clearMultiDamageAnimation(CharacterStatus* self);
        static void setMultiDamageAnimation(CharacterStatus* self);
        static bool isMultiDamageAnimation2(CharacterStatus* self);
        static void clearMultiDamageAnimation2(CharacterStatus* self);
        static void setMultiDamageAnimation2(CharacterStatus* self);
        static bool isMultiDamageAnimation1(CharacterStatus* self);
        static void clearMultiDamageAnimation1(CharacterStatus* self);
        static void setMultiDamageAnimation1(CharacterStatus* self);
        static bool isDisappearAnimation(CharacterStatus* self);
        static void setDisappearAnimation(CharacterStatus* self);
        static void clearDisappearAnimation(CharacterStatus* self);
        static bool isDeathAnimation(CharacterStatus* self);
        static void setDeathAnimation(CharacterStatus* self);
        static void clearDeathAnimation(CharacterStatus* self);
        static bool isAddDamageAnimation(CharacterStatus* self);
        static void setAddDamageAnimation(CharacterStatus* self);
        static void clearAddDamageAnimation(CharacterStatus* self);
        static bool isDamageAnimation(CharacterStatus* self);
        static void clearDamageAnimation(CharacterStatus* self);
        static void setNoneAnimation(CharacterStatus* self);
        static void clearMenuStatusFlag(CharacterStatus* self);
        static bool isMenuStatusFlag(CharacterStatus* self, status::HaveStatusInfo::DiffStatus status);
        static void clearMenuStatusFlag(CharacterStatus* self, status::HaveStatusInfo::DiffStatus status);
        static void changeMonsterWithEtc(CharacterStatus* self, int index);
        static void changeMonsterWithoutHpMp(CharacterStatus* self, int index);
        static void changeMonster(CharacterStatus* self, int index);
        static void cleanup(CharacterStatus* self);
        static void setup(CharacterStatus* self, dq5::level::CharacterType type, int group, int index);
        static bool isActionEnableWithoutDragoram(CharacterStatus* self);
        static bool isActionEnable(CharacterStatus* self);
        virtual void printAction(CharacterStatus* self);
        virtual void print(CharacterStatus* self);
        static void setMenuStatusFlag(CharacterStatus* self, status::HaveStatusInfo::DiffStatus status);
        virtual ~CharacterStatus() = default;
        CharacterStatus();
    };

} 
