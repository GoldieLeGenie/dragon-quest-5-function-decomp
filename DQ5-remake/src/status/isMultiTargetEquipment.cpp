#include "status/isMultiTargetEquipment.h"
#include "status/CharacterStatus.h"
#include "status/HaveEquipment.h"


bool status::isMultiTargetEquipment(status::CharacterStatus* actor)
{
    if (!actor)
        return false;

    status::HaveEquipment* equip = &actor->haveStatusInfo_.haveEquipment_;

    return status::HaveEquipment::isEquipment(equip, 23) ||
        status::HaveEquipment::isEquipment(equip, 53) ||
        status::HaveEquipment::isEquipment(equip, 54) ||
        status::HaveEquipment::isEquipment(equip, 55) ||
        status::HaveEquipment::isEquipment(equip, 56) ||
        status::HaveEquipment::isEquipment(equip, 59) ||
        status::HaveEquipment::isEquipment(equip, 57) ||
        status::HaveEquipment::isEquipment(equip, 58) ||
        status::HaveEquipment::isEquipment(equip, 59) || 
        status::HaveEquipment::isEquipment(equip, 60) ||
        status::HaveEquipment::isEquipment(equip, 61);
}

