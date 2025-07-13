#include "status/UseAction.h"
#include "status/HaveAction.h"
#include "status/BaseAction.h"
#include "status/HaveStatusInfo.h"
#include "status/UseActionParam.h"
#include "dq5/ActionParam.h"
#include "args/ExcelBinaryData.h"
#include "ar/rand.h"

// Définition de la variable statique
int status::UseAction::useActionParam_;
bool status::UseAction::effectValueMax_; 
dq5::level::ActionParam* status::UseAction::actionParam2_ = nullptr;


bool status::UseAction::isForceE(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record) return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (actionParam2_->byte_3 >> 3) & 0x1;
}


bool status::UseAction::isMultiF(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record) return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    
    return (actionParam2_->byte_3 >> 4) & 0x1;
}

int status::UseAction::getEffectValueToPlayer(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record) return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    int max = actionParam2_->PlayerMax;

    if (!effectValueMax_) {
        int min = actionParam2_->PlayerMin;
        if (max < min) return min;
        return ar::rand(max - min + 1) + min;
    }
    return max;
}

int status::UseAction::getEffectValueToMonster(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record) return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    int max = actionParam2_->MonsterMax;

    if (!effectValueMax_) {
        int min = actionParam2_->MonsterMin;
        if (max < min) return min; 
        return ar::rand(max - min + 1) + min;
    }

    return max;
}

bool status::UseAction::isSpecialSelectTarget(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );
    if (!record) return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 6) & 0x1;
 
}

bool status::UseAction::isCrossFire(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );
    if (!record) return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (actionParam2_->byte_2 & 0x01);

}

status::UseAction::UseArea status::UseAction::getUseArea(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return status::UseAction::UseArea::Zero; 

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    // Table d'équivalence extraite de .rodata:dword_256AB0
    static constexpr status::UseAction::UseArea useAreaMap[8] = {
        status::UseAction::UseArea::Horse,          // 0
        status::UseAction::UseArea::UsuallyHorse,   // 1
        status::UseAction::UseArea::UsuallyHorse,
        status::UseAction::UseArea::BattleHorse,    // 2
        status::UseAction::UseArea::One,            // 3
        status::UseAction::UseArea::One,            // 5
        status::UseAction::UseArea::Group,          // 6
        status::UseAction::UseArea::All             // 7
    };

    uint8_t raw = reinterpret_cast<uint8_t*>(record)[6]; 
    uint8_t index = ((raw >> 3) & 0x07) ^ 0x04;

    return useAreaMap[index];
}


void status::UseAction::getAIType(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record) {
        actionParam2_ = nullptr;
        return;
    }

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

}

dq5::level::ActionParam::DEFENCE status::UseAction::getActionDefenceKind(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

   
    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    return static_cast<dq5::level::ActionParam::DEFENCE>(actionParam2_->magictype);

}



dq5::level::ActionParam::DEFENCE status::UseAction::getActionDefenceKind2(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    return static_cast<dq5::level::ActionParam::DEFENCE>(actionParam2_->magictype2);

}


dq5::level::ActionParam::DEFENCETYPE status::UseAction::getActionDefenceType(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    uint8_t raw = actionParam2_->byte_5;
    uint8_t index = (raw >> 3) & 0x07;

    return static_cast<dq5::level::ActionParam::DEFENCETYPE>(index);
}



status::UseAction::ActionType status::UseAction::getActionType(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    uint8_t raw = actionParam2_->type;
    uint8_t index = raw - 1;

    if (index >= 4)
        return status::UseAction::ActionType::ActionTypeNone;

    static constexpr status::UseAction::ActionType actionTypeMap[4] = {
        status::UseAction::ActionType::ActionTypeMagic,
        status::UseAction::ActionType::ActionTypeSkill,
        status::UseAction::ActionType::ActionTypeMagic,
        status::UseAction::ActionType::ActionTypeSkill
    };

    return actionTypeMap[index];
}

status::UseAction::DamageType status::UseAction::getDamageType(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );



    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    uint8_t index = static_cast<uint8_t>(actionParam2_->typeID) - 1;

    static constexpr status::UseAction::DamageType damageTypeMap[44] = {
        status::UseAction::DamageType::DamageTypeNone,
        status::UseAction::DamageType::DamageTypeDamage,
        status::UseAction::DamageType::DamageTypeRecovery,
        status::UseAction::DamageType::DamageTypeInstantDeath,
        status::UseAction::DamageType::DamageTypeRebirth,
        status::UseAction::DamageType::DamageTypeAddMp,
        status::UseAction::DamageType::DamageTypeSubMp,
        status::UseAction::DamageType::DamageTypeStatusClear,
        status::UseAction::DamageType::DamageTypeAstoron,
        status::UseAction::DamageType::DamageTypeSpazz,
        status::UseAction::DamageType::DamageTypeSleep,
        status::UseAction::DamageType::DamageTypeManusa,
        status::UseAction::DamageType::DamageTypeBaikiruto,
        status::UseAction::DamageType::DamageTypeFubaha,
        status::UseAction::DamageType::DamageTypeMahokanta,
        status::UseAction::DamageType::DamageTypeMahosute,
        status::UseAction::DamageType::DamageTypeMosyasu,
        status::UseAction::DamageType::DamageTypePowerSave,
        status::UseAction::DamageType::DamageTypeMahoton,
        status::UseAction::DamageType::DamageTypeDragoram,
        status::UseAction::DamageType::DamageTypeConfusion,
        status::UseAction::DamageType::DamageTypePath1,
        status::UseAction::DamageType::DamageTypePoison,
        status::UseAction::DamageType::DamageTypeMOUDOKU,
        status::UseAction::DamageType::DamageTypeDefenceChange,
        status::UseAction::DamageType::DamageTypeAgilityChange,
        status::UseAction::DamageType::DamageTypeFeather,
        status::UseAction::DamageType::DamageTypeKOORITSUKI,
        status::UseAction::DamageType::DamageTypeLight,
        status::UseAction::DamageType::DamageTypeWOOLGUARD,
        status::UseAction::DamageType::DamageTypeMAHOTAN,
        status::UseAction::DamageType::DamageTypeMAHOKITE,
        status::UseAction::DamageType::DamageTypeOIKAZE,
        status::UseAction::DamageType::DamageTypeHENSHIN,
        status::UseAction::DamageType::DamageTypeMIKAWASHIKYAKU,
        status::UseAction::DamageType::DamageTypeSEIREINOUTA,
        status::UseAction::DamageType::DamageTypeODORIFUUJI,
        status::UseAction::DamageType::DamageTypeKUSUGURINOKEI,
        status::UseAction::DamageType::DamageTypeMDEF_HENKA,
        status::UseAction::DamageType::DamageTypeDefence,
        status::UseAction::DamageType::DamageTypeFizzleZone,
        status::UseAction::DamageType::DamageTypeTimeStop,
        status::UseAction::DamageType::DamageTypeHIKARI,
        status::UseAction::DamageType::DamageTypeMAX,
    };
    return damageTypeMap[index];
}


int status::UseAction::getEffectValueForAvarage(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<int>(actionParam2_->PlayerMax) + static_cast<int>(actionParam2_->PlayerMin)) >> 1;
}


int status::UseAction::getEffectValueToMonsterMin(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->MonsterMin;
}


int status::UseAction::getEffectValueToPlayerMin(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->PlayerMin;
}


int status::UseAction::getMenuMessage(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->menuMes;
}


int status::UseAction::getUseMp(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return 0;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->useMP;
}


void status::UseAction::getUseType(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
}


int status::UseAction::getWordDBIndex(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionIndex; 
}


bool status::UseAction::isActorAlive(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_4) >> 1) & 0x01;
}

bool status::UseAction::isAddHp(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_3) >> 6) & 0x01;
}

bool status::UseAction::isAddMp(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->byte_3 < 0;
}

bool status::UseAction::isAstoron(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 1) & 0x01;
}


bool status::UseAction::isBaikiruto(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 6) & 0x01;
}


bool status::UseAction::isBattleUse(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_4) >> 5) & 0x01;
}

bool status::UseAction::isBigDefence(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 3) & 0x01;
}


bool status::UseAction::isDamageC(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_3) >> 1) & 0x01;
}

bool status::UseAction::isDamageD(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_3) >> 2) & 0x01;
}

bool status::UseAction::isErrorA(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->byte_2 < 0;
}

bool status::UseAction::isErrorB(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (actionParam2_->byte_3 & 0x01);
}


bool status::UseAction::isFubaha(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 1) & 0x01;
}

bool status::UseAction::isJouk(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 4) & 0x01;
}


bool status::UseAction::isKaishin(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->byte_1 < 0;
}

bool status::UseAction::isMahokanta(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 2) & 0x01;
}

bool status::UseAction::isMahokite(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 3) & 0x01;
}

bool status::UseAction::isMahoton(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_4) >> 4) & 0x01;
}


bool status::UseAction::isManeEnable(int actionIndex)
{
    return 0;
}

bool status::UseAction::isManusa(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 2) & 0x01;
}


bool status::UseAction::isMosyasAction(int actionIndex)
{
    return 0;
}

bool status::UseAction::isOmakeH(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_3) >> 5) & 0x01;
}

bool status::UseAction::isPowerSave(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_1) >> 5) & 0x01;
}


bool status::UseAction::isReleaseConfusion(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 4) & 0x01;
}


bool status::UseAction::isResultNeverChange(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );


    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    using uslong = unsigned long;

    uslong playerSuccess = actionParam2_->playerSuccess;
    uslong monsterSuccess = actionParam2_->monsterSuccess;
    uslong playerFailed = actionParam2_->playerFailed;
    uslong monsterFailed = actionParam2_->monsterFailed;

    uslong values[6] = {
        playerSuccess,
        playerSuccess,
        monsterSuccess,
        monsterSuccess,
        playerFailed,
        monsterFailed
    };

    for (int i = 1; i < 6; ++i) {
        playerSuccess += values[i];
    }

    return playerSuccess == 0;
}


bool status::UseAction::isSelectTarget(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_2) >> 6) & 0x01;
}


bool status::UseAction::isTargetAlive(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_4) >> 1) & 0x01;
}


bool status::UseAction::isTargetDeadOrAlive(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return (static_cast<uint8_t>(actionParam2_->byte_4) >> 1) & 0x01;
}


bool status::UseAction::isUse(int actionIndex, HaveStatusInfo_0* actorStatusInfo) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    status::UseAction::actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);
    unsigned int useMP = actionParam2_->useMP;

    unsigned int actorMp = status::HaveStatusInfo::getMp(actorStatusInfo);

    if (useMP > actorMp) {
        record = dq5::level::ActionParam::binary_.getRecord(
            actionIndex,
            dq5::level::ActionParam::addr_,
            dq5::level::ActionParam::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
        );

        if (!record)
            return false;

        status::UseAction::actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

        return actionParam2_->useMP == 255 && actorMp != 0;
    }

    return true;
}


bool status::UseAction::isUsuallyUse(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->byte_4 < 0;
}

bool status::UseAction::isYaibaArmor(int actionIndex) {
    void* record = dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    );

    if (!record)
        return false;

    actionParam2_ = reinterpret_cast<dq5::level::ActionParam*>(record);

    return actionParam2_->byte_1 & 1;
}

void status::UseAction::execUse(UseActionParam* useActionParam) {
    if (!useActionParam)
        return;

    useActionParam_ = reinterpret_cast<int>(useActionParam);

    bool isBattleMode = HaveAction::isBattleMode();

    /*status::BaseAction::exec(useActionParam, isBattleMode);*/
}