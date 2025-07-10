#include "btl/BattleSelectTarget.h"
#include "btl/BattleSelectTargetParam.h"
#include "status/CharacterStatus.h"


void btl::BattleSelectTargetParam::setSourceCharacterStatus(
    btl::BattleSelectTargetParam* self,
    int index,
    status::CharacterStatus* characterStatus)
{
    self->sourceCharacterStatus[index] = characterStatus;
}

