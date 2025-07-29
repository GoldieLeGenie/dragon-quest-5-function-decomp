#include "status/isDoubleAction.h"
#include "status/CharacterStatus.h"
#include "status/HaveEquipment.h"
#include "status/UseAction.h"
#include "status/isDoubleAction.h"

bool status::isDoubleAction(status::CharacterStatus* actor, int actionIndex)
{
    if (!actor)
        return false;

    if (!status::UseAction::isMultiF(actionIndex))
        return false;

    status::HaveEquipment* p_haveEquipment = &actor->haveStatusInfo_.haveEquipment_;

    if (!status::HaveEquipment::isEquipment(p_haveEquipment, 17) &&
        !status::HaveEquipment::isEquipment(p_haveEquipment, 52) &&
        !status::HaveEquipment::isEquipment(p_haveEquipment, 258))
    {
        return status::HaveEquipment::isEquipment(p_haveEquipment, 259);
    }

    return true;
}


