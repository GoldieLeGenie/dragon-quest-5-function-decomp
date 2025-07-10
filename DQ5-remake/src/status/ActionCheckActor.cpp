#include "status/ActionCheckActor.h"
#include "status/UseActionParam.h"
#include "status/UseAction.h"
#include "status/HaveStatusInfo.h"
#include "status/HaveEquipment.h"
#include "ar/rand.h"


void status::ActionCheckActor::checkActorImmediateItem(status::UseActionParam* useActionParam)
{
    status::CharacterStatus* actorCharacterStatus; 
    int actionIndex; 
    status::HaveStatusInfo* p_haveStatusInfo; 
    status::CharacterStatus* actor; 

    actorCharacterStatus = useActionParam->actorCharacterStatus_;
    actionIndex = useActionParam->actionIndex_;
    p_haveStatusInfo = &useActionParam->actorCharacterStatus_->haveStatusInfo_;
    actor = useActionParam->targetCharacterStatus_[useActionParam->currentTargetIndex_];
    status::HaveStatusInfo::setImmediateDeathItem((status::HaveStatusInfo_0*)p_haveStatusInfo, false);
    if (actor && !status::UseActionFlag::eventBattle_ && status::UseAction::isForceE(actionIndex))
    {
        if (status::HaveEquipment::isEquipment(&actorCharacterStatus->haveStatusInfo_.haveEquipment_, 51) && !ar::rand(6))
            status::HaveStatusInfo::setImmediateDeathItem((status::HaveStatusInfo_0*)p_haveStatusInfo, true);
        if (status::HaveEquipment::isEquipment(&actorCharacterStatus->haveStatusInfo_.haveEquipment_, 64) && !ar::rand(6))
            status::HaveStatusInfo::setImmediateDeathItem((status::HaveStatusInfo_0*)p_haveStatusInfo, true);
    }
}


bool status::ActionCheckActor::isAlive(status::UseActionParam* self)
{
    int actionIndex = self->actionIndex_;

    // Cas spécial : action 491 ne vérifie pas la vie
    if (actionIndex == 491)
        return true;

    status::CharacterStatus* actor = self->actorCharacterStatus_;
    status::CharacterStatus* target = self->targetCharacterStatus_[self->currentTargetIndex_];

    // Si l'acteur est sa propre cible
    if (actor == target)
    {
        // Si ni acteur ni cible ne sont nuls
        if (actor && target)
        {
            // On retourne vrai s’il n’est pas mort
            return !status::HaveStatusInfo::isDeath(
                reinterpret_cast<const status::HaveStatusInfo_0*>(&actor->haveStatusInfo_));
        }
    }

    return true;
}

