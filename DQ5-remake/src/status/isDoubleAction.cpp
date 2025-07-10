#include "status/isDoubleAction.h"
#include "status/CharacterStatus.h"
#include "status/HaveEquipment.h"
#include "status/UseAction.h"
#include "status/isDoubleAction.h"

bool status::isDoubleAction(status::CharacterStatus* actor, int actionIndex)
{
    if (!actor)
        return false;

    // Vérifie si l'action autorise une exécution multiple (type multi-hit)
    if (!status::UseAction::isMultiF(actionIndex))
        return false;

    status::HaveEquipment* p_haveEquipment = &actor->haveStatusInfo_.haveEquipment_;

    // Si le personnage porte un équipement spécifique, il ne peut pas doubler
    if (!status::HaveEquipment::isEquipment(p_haveEquipment, 17) &&
        !status::HaveEquipment::isEquipment(p_haveEquipment, 52) &&
        !status::HaveEquipment::isEquipment(p_haveEquipment, 258))
    {
        // Peut doubler seulement si l'équipement 259 est porté
        return status::HaveEquipment::isEquipment(p_haveEquipment, 259);
    }

    // Par défaut : peut doubler
    return true;
}


