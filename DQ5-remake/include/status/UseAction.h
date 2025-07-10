#pragma once

#include "status/UseActionParam.h"
#include "dq5/ActionParam.h"

namespace status {

    struct UseAction {

        enum UseArea : int32_t
        {
            Zero = 0x0,
            One = 0x1,
            Group = 0x2,
            All = 0x3,
            Horse = 0x4,
            UsuallyHorse = 0x5,
            BattleHorse = 0x6,
        };

        enum ActionType : int32_t
        {
            ActionTypeNone = 0x0,
            ActionTypeMagic = 0x1,
            ActionTypeSkill = 0x2,
        };

        enum DamageType : int32_t
        {
            DamageTypeNone = 0x0,
            DamageTypeDamage = 0x1,
            DamageTypeRecovery = 0x2,
            DamageTypeInstantDeath = 0x3,
            DamageTypeRebirth = 0x4,
            DamageTypeAddMp = 0x5,
            DamageTypeSubMp = 0x6,
            DamageTypeStatusClear = 0x7,
            DamageTypeAstoron = 0x8,
            DamageTypeSpazz = 0x9,
            DamageTypeSleep = 0xA,
            DamageTypeManusa = 0xB,
            DamageTypeBaikiruto = 0xC,
            DamageTypeFubaha = 0xD,
            DamageTypeMahokanta = 0xE,
            DamageTypeMahosute = 0xF,
            DamageTypeMosyasu = 0x10,
            DamageTypePowerSave = 0x11,
            DamageTypeMahoton = 0x12,
            DamageTypeDragoram = 0x13,
            DamageTypeConfusion = 0x14,
            DamageTypePath1 = 0x15,
            DamageTypePoison = 0x16,
            DamageTypeMOUDOKU = 0x17,
            DamageTypeDefenceChange = 0x18,
            DamageTypeAgilityChange = 0x19,
            DamageTypeFeather = 0x1A,
            DamageTypeKOORITSUKI = 0x1B,
            DamageTypeLight = 0x1C,
            DamageTypeWOOLGUARD = 0x1D,
            DamageTypeMAHOTAN = 0x1E,
            DamageTypeMAHOKITE = 0x1F,
            DamageTypeOIKAZE = 0x20,
            DamageTypeHENSHIN = 0x21,
            DamageTypeMIKAWASHIKYAKU = 0x22,
            DamageTypeSEIREINOUTA = 0x23,
            DamageTypeODORIFUUJI = 0x24,
            DamageTypeKUSUGURINOKEI = 0x25,
            DamageTypeMDEF_HENKA = 0x26,
            DamageTypeDefence = 0x27,
            DamageTypeFizzleZone = 0x28,
            DamageTypeTimeStop = 0x29,
            DamageTypeHIKARI = 0x2A,
            DamageTypeMAX = 0x2B,
        };


        static UseArea getUseArea(int actionIndex);
        static int useActionParam_;
        static bool effectValueMax_;  

        static bool isForceE(int actionIndex);
        static bool isMultiF(int actionIndex);
        static int getEffectValueToPlayer(int actionIndex);
        static int getEffectValueToMonster(int actionIndex);
        static bool isSpecialSelectTarget(int actionIndex);
        static bool isCrossFire(int actionIndex);
        static dq5::level::ActionParam* actionParam2_;
        static void getAIType(int actionIndex);
        static dq5::level::ActionParam::DEFENCE getActionDefenceKind(int actionIndex);
        static dq5::level::ActionParam::DEFENCE getActionDefenceKind2(int actionIndex);
        static dq5::level::ActionParam::DEFENCETYPE getActionDefenceType(int actionIndex);
        static status::UseAction::ActionType getActionType(int actionIndex);
        status::UseAction::DamageType getDamageType(int actionIndex);
        static int getEffectValueForAvarage(int actionIndex);
        static int getEffectValueToMonsterMin(int actionIndex);
        static int getEffectValueToPlayerMin(int actionIndex);
        static int getMenuMessage(int actionIndex);
        static int getUseMp(int actionIndex);
        static void getUseType(int actionIndex);
        static int getWordDBIndex(int actionIndex);
        static bool isActorAlive(int actionIndex);
        static bool isAddHp(int actionIndex);
        static bool isAddMp(int actionIndex);
        static bool isAstoron(int actionIndex);
        static bool isBaikiruto(int actionIndex);
        static bool isBattleUse(int actionIndex);
        static bool isBigDefence(int actionIndex);
        static bool isDamageC(int actionIndex);
        static bool isDamageD(int actionIndex);
        static bool isErrorA(int actionIndex);
        static bool isErrorB(int actionIndex);
        static bool isFubaha(int actionIndex);
        static bool isJouk(int actionIndex);
        static bool isKaishin(int actionIndex);
        static bool isMahokanta(int actionIndex);
        static bool isMahokite(int actionIndex);
        static bool isMahoton(int actionIndex);
        static bool isManeEnable(int actionIndex);
        static bool isManusa(int actionIndex);
        static bool isMosyasAction(int actionIndex);
        static bool isOmakeH(int actionIndex);
        static bool isPowerSave(int actionIndex);
        static bool isReleaseConfusion(int actionIndex);
        static bool isResultNeverChange(int actionIndex);
        static bool isSelectTarget(int actionIndex);
        static bool isTargetAlive(int actionIndex);
        static bool isTargetDeadOrAlive(int actionIndex);
        static bool isUse(int actionIndex, status::HaveStatusInfo_0* actorStatusInfo);
        static bool isUsuallyUse(int actionIndex);
        static bool isYaibaArmor(int actionIndex);
        static void execUse(UseActionParam* useActionParam);
    };
    
} // namespace status
