#pragma once
#include "status/CharacterStatus.h"

namespace btl {

    struct BattleSelectTargetParam {
        status::CharacterStatus* sourceCharacterStatus[18];  // 0x00 - 0x48
        status::CharacterStatus* targetCharacterStatus[18];  // 0x48 - 0x90
        status::CharacterStatus* actorCharacterStatus_;      // 0x90
        int sourceCount_;                                    // 0x94
        int targetCount_;                                    // 0x98
        int actionIndex_;                                    // 0x9C
        int targetGroup_;                                    // 0xA0
        int targetIndex_;                                    // 0xA4
        void setSourceCharacterStatus(btl::BattleSelectTargetParam*, int index, status::CharacterStatus* characterStatus);

    };

}  
