#include "status/ActionEffectValue.h"
#include "status/UseActionParam.h"
#include "status/UseAction.h"
#include "status/getUsuallyAttackEffectValue.h"
#include "status/isDoubleAction.h"
#include "status/HaveBattleStatus.h"
#include "status/HaveStatusInfo.h"
#include "status/UseActionFlag.h"
#include "status/BaseAction.h"
#include "status/isNormalAttack.h"
#include "status/getRandomVariation.h"
#include "ar/rand.h"


bool status::ActionEffectValue::specialTargetEffectValue_ = false;


void __fastcall status::ActionEffectValue::setEffectValue(UseActionParam* useActionParam, bool flag)
{
    // Étape 1 : vérifier s’il s’agit d’une attaque multi-cible ou multiple
    status::ActionEffectValue::checkMultiAttack(useActionParam);

    // Étape 2 : si le flag `specialTargetEffectValue_` n’est pas encore activé
    if (!status::ActionEffectValue::specialTargetEffectValue_)
    {
        // On l’active avec la valeur passée en paramètre
        status::ActionEffectValue::specialTargetEffectValue_ = flag;

        // Ensuite on applique plusieurs effets standards :
        status::ActionEffectValue::setEffectValueNormalAttack(useActionParam); // dégât normal
        status::ActionEffectValue::setEffectValueBasic(useActionParam);        // effets de base
        status::ActionEffectValue::setEffectValueException(useActionParam);    // cas spéciaux ou overrides
    }
}

void status::ActionEffectValue::checkMultiAttack(UseActionParam* useActionParam) {
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;

    // Si l'action est une double action, que l'état est multi-attaque, et qu'on est sur la 2e cible
    if (status::isDoubleAction(actor, useActionParam->actionIndex_) &&
        status::HaveStatusInfo::isMultiAttack(reinterpret_cast<status::HaveStatusInfo_0*>(&actor->haveStatusInfo_)) &&
        useActionParam->currentTargetIndex_ == 1)
    {
        // Réinitialise la valeur d'effet spéciale (sera recalculée)
        status::ActionEffectValue::specialTargetEffectValue_ = 0;
    }
}

void __fastcall status::ActionEffectValue::setEffectValueNormalAttack(status::UseActionParam* useActionParam)
{
    if (!useActionParam)
        return;

    // Récupère l'attaquant (acteur)
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    if (!actor)
        return;

    // Récupère la cible actuelle
    int targetIndex = useActionParam->currentTargetIndex_;
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[targetIndex];
    if (!target)
        return;

    // Calcule la valeur d'effet de l'attaque normale
    int value = status::getUsuallyAttackEffectValue(actor, target);

    // Affecte la valeur dans les structures du paramètre d’action
    useActionParam->effectValue_ = value;
    status::UseActionParam::setPlayerEffectValue(useActionParam, value);
    status::UseActionParam::setMonsterEffectValue(useActionParam, value);
}



void status::ActionEffectValue::setEffectValueBasic(status::UseActionParam* useActionParam) {
    if (!useActionParam) return;

    auto* target = useActionParam->targetCharacterStatus_[useActionParam->currentTargetIndex_];
    if (!target) return;

    int actionIndex = useActionParam->actionIndex_;
    auto* actor = useActionParam->actorCharacterStatus_;

    int valueToPlayer = status::UseAction::getEffectValueToPlayer(actionIndex);
    int valueToMonster = status::UseAction::getEffectValueToMonster(actionIndex);

    if ((valueToPlayer + valueToMonster) == 0)
        return;

    status::UseActionParam::setPlayerEffectValue(useActionParam, valueToPlayer);
    status::UseActionParam::setMonsterEffectValue(useActionParam, valueToMonster);

    int rawEffectValue = 0;
    if (actor) {
        rawEffectValue = status::getUsuallyAttackEffectValue(actor, target);
    }
    else {
        // Si actor == nullptr, accède quand même à l'offset brut selon le type de target
        int offset = (target->characterType_ == dq5::level::CharacterType::PLAYER) ? 140 : 144;
        rawEffectValue = *reinterpret_cast<int*>(reinterpret_cast<char*>(&useActionParam->actorCharacterStatus_) + offset);
    }

    useActionParam->effectValue_ = rawEffectValue;
}



void status::ActionEffectValue::setEffectValueException(status::UseActionParam* useActionParam) {
    if (!useActionParam) return;

    int actionIndex = useActionParam->actionIndex_;
    status::CharacterStatus* actor = useActionParam->actorCharacterStatus_;
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[useActionParam->currentTargetIndex_];
    int value = useActionParam->effectValue_;

    /*if (actionIndex == 66 && actor) {
        actor->haveBattleStatus_.setupParupunteAction();
        useActionParam->addActionIndex_ = actor->haveBattleStatus_.actionIndex_;
        useActionParam->preActionIndex_ = 66;
    }*/

    if (status::isNormalAttack(actionIndex)) {
        if (status::HaveStatusInfo::isKaishin(reinterpret_cast<status::HaveStatusInfo_0*>(&actor->haveStatusInfo_))) {
            value = useActionParam->effectValue_;  // potentiel critique, mais valeur inchangée ici
        }

        status::UseActionParam::setPlayerEffectValue(useActionParam, value);
        status::UseActionParam::setMonsterEffectValue(useActionParam, value);
        status::ActionEffectValue::setEffectValueWithDoubleAttack(useActionParam);
        return;
    }

    int result = 0;

    // Spécial : valeurs entre 162–172
    if (actionIndex >= 162 && actionIndex <= 172) {
        result = value * 125 / 100;
    }
    else if (actionIndex == 161) {
        result = value * 150 / 100;
    }
    else if (actionIndex == 106) {
        result = value;
        if (target && !target->haveBattleStatus_.metal_)
            result += 60;
    }
    else if (actionIndex == 107) {
        result = (value == 1) ? 1 : value / 2;
    }
    else if (actionIndex == 259) {
        status::UseActionFlag::timeReverseFlag_ = true;
        return;
    }
    else if (actionIndex == 411 || actionIndex == 421) {
        result = value / 4;
    }
    else if (actionIndex == 478 && actor) {
        int hpMax = status::HaveStatusInfo::getHpMax(reinterpret_cast<status::HaveStatusInfo_0*>(&actor->haveStatusInfo_));
        result = (hpMax > 5) ? hpMax / 6 : 1;
    }
    else if (actionIndex == 'R') { // code ASCII 'R' = 82
        useActionParam->addActionIndex_ = 480;
        useActionParam->preActionIndex_ = actionIndex;
        return;
    }
    else if (actionIndex == 'S' || actionIndex == 'T' || actionIndex == 'U') {
        result = value;
    }
    else if (actionIndex == 'V' && actor && target) {
        int atk = status::HaveStatusInfo::getAttack(reinterpret_cast<status::HaveStatusInfo_0*>(&actor->haveStatusInfo_), 0);
        result = status::getRandomVariation(atk, 0, 5);
        if (target->haveStatusInfo_.actionDefence_.zaki_ == 3)
            result = ar::rand(2);
    }
    else {
        return; // Aucun effet spécifique
    }

    // Applique la valeur calculée
    useActionParam->effectValue_ = result;
    status::UseActionParam::setPlayerEffectValue(useActionParam, result);
    status::UseActionParam::setMonsterEffectValue(useActionParam, result);
}


void status::ActionEffectValue::setEffectValueWithDoubleAttack(status::UseActionParam* useActionParam)
{
    status::HaveEquipment* p_haveEquipment;

    p_haveEquipment = &useActionParam->actorCharacterStatus_->haveStatusInfo_.haveEquipment_;

    // Appels non statiques aux méthodes d'instance
    bool hasEquip17 = status::HaveEquipment::isEquipment(p_haveEquipment, 17);
    bool hasEquip259 = status::HaveEquipment::isEquipment(p_haveEquipment, 259);
    bool hasEquip258 = status::HaveEquipment::isEquipment(p_haveEquipment, 258);

    int multiplier = hasEquip17 ? 2 : 1;
    if (!hasEquip259)
        multiplier = hasEquip17 ? 1 : 0;

    // Si au moins un des équipements double attaque (17, 258) est équipé
    if (multiplier + hasEquip258 > 0)
    {
        int reducedEffect = useActionParam->effectValue_ * 75 / 100;
        useActionParam->effectValue_ = reducedEffect;

        status::UseActionParam::setPlayerEffectValue(useActionParam, reducedEffect);
        status::UseActionParam::setMonsterEffectValue(useActionParam, reducedEffect);
        
    }
}
