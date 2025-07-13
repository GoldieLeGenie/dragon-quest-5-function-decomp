#include "status/HaveBattleStatus.h"
#include "status/StageAttribute.h"
#include "ar/rand.h"
#include "dq5/MonsterData.h"



bool status::HaveBattleStatus::eventFlag_ = false;

int status::parupunteEffects[25] = {
        0x1B9, 0x1BA, 0x1BC, 0x1BD, 0x1BE, 0x1BF, 0x1C1, 0x1C2,
        0x1C3, 0x1C4, 0x1C5, 0x1C6, 0x1C7, 0x1C8, 0x1C9, 0x1CA,
        0x1CB, 0x1CC, 0x1CD, 0x1CE, 0x1CF, 0x1D0, 0x1D1, 0x1D2, 0x1D3
};

bool status::isParupunteCarriageEnabled = false;

int status::parupunteEventEffects[10] = {
    0xBE,0xBF,0xC6,0xC9,0xCC,0xD0,0xD1,0xD2,0xD4,0xD6
};

int status::HaveBattleStatus::getParupunteAction(HaveBattleStatus*, bool event) {
    if (event) {
        int index = ar::rand(10);
        return parupunteEventEffects[index];
    }
    else {
        int index = ar::rand(25);
        return parupunteEffects[index];
    }
}


void status::HaveBattleStatus::setActionFailed(HaveBattleStatus* self, int actionIndex) {
    const int originalPatternIndex = self->patternIndex_;

    for (int i = 0; i < 6; ++i) {
        self->patternIndex_ = i;
        status::HaveBattleStatus::setActionIndexForMonster(self);
        self->patternFailedFlag_.set(i);
    }

    self->patternIndex_ = originalPatternIndex;
}


void status::HaveBattleStatus::setActionIndexForMonster(HaveBattleStatus* self) {
    // Récupération du record depuis les données binaires
    void* record = dq5::level::MonsterData::binary_.getRecord(
        self->index_,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
    );

    if (!record) return;  // Sécurité si record non trouvé

    // Cast vers la bonne structure
    dq5::level::MonsterData* monsterData = static_cast<dq5::level::MonsterData*>(record);

    // Sauvegarde dans la variable globale
    dq5::level::monsterData2_ = monsterData;

    // On déduit l'action selon le patternIndex_
    uint16_t actionValue = 0;
    switch (self->patternIndex_) {
    case 0: actionValue = monsterData->action1; break;
    case 1: actionValue = monsterData->action2; break;
    case 2: actionValue = monsterData->action3; break;
    case 3: actionValue = monsterData->action4; break;
    case 4: actionValue = monsterData->action5; break;
    case 5: actionValue = monsterData->action6; break;
    default: return;  // Rien à faire
    }

    self->actionIndex_ = static_cast<int>(actionValue);
}

