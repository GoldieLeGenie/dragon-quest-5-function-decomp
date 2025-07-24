#include "status/HaveBattleStatus.h"
#include "status/StageAttribute.h"
#include "ar/rand.h"
#include "dq5/MonsterData.h"
#include "dq5/CharacterInitData.h"



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
    dq5::level::MonsterData* monsterData2_ = monsterData;

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


void status::HaveBattleStatus::setup(HaveBattleStatus* self,dq5::level::CharacterType type,int group,int index)
{
    self->groupIndex_ = group;
    self->index_ = index;
    self->escape_ = 0;
    self->actionIndex_ = 0;
    self->actionCount_ = 0;
    self->patternFailedFlag_.set(0);
    self->disablePattern_.set(0);
    self->disablePattern2nd_.set(0);
    self->selectCommand_ = status::HaveBattleStatus::SelectCommand::NoSelect;
    self->type_ = type;
    self->change_ = 0;
    self->mosyasActionCount_ = 0;
    memset(self->mosyasAction_, 71, sizeof(self->mosyasAction_));

    int monsterIndexForNpc = 0;

    if (type == dq5::level::CharacterType::NONE) 
    {
        dq5::level::CharacterInitData* characterInitData2_ =
            static_cast<dq5::level::CharacterInitData*>(
                dq5::level::CharacterInitData::binary_.getRecord(
                    index,
                    dq5::level::CharacterInitData::addr_,
                    dq5::level::CharacterInitData::filename_[0],
                    static_cast<ar::File::LoadSwitch>(dq5::level::CharacterInitData::loadSwitch_)
                ));

        monsterIndexForNpc = characterInitData2_->actionMonsterID;
        self->monsterIndexForNpc_ = monsterIndexForNpc;
    }
    else
    {
        monsterIndexForNpc = self->monsterIndexForNpc_;
    }

    if (monsterIndexForNpc != 0)
    {
        if (type == dq5::level::CharacterType::PLAYER)
        {
            self->type_ = dq5::level::CharacterType::MONSTER;
            self->index_ = monsterIndexForNpc;
            status::HaveBattleStatus::setupMonster(self);
            return;
        }
        if (type == dq5::level::CharacterType::MONSTER)
        {
            status::HaveBattleStatus::setupMonster(self);
            return;
        }
        return;
    }

    if (type == dq5::level::CharacterType::NONE)
    {
        self->dragon_ = 0;
        self->brains_ = 0;
        self->slime_ = 256;
        self->multi_ = 0;
    }
}


void status::HaveBattleStatus::setupMonster(HaveBattleStatus* self)
{
    
    dq5::level::MonsterData* Record;

    Record = static_cast<dq5::level::MonsterData*>(
        dq5::level::MonsterData::binary_.getRecord(
            self->index_,
            dq5::level::MonsterData::addr_,
            dq5::level::MonsterData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
        ));


    dq5::level::MonsterData* monsterData2_ = Record;


    const uint8_t byte2 = static_cast<uint8_t>(Record->byte_2);
    const uint8_t byte1 = static_cast<uint8_t>(Record->byte_1);
    const uint8_t byte8 = static_cast<uint8_t>(Record->byte_8);
    const uint8_t byte9 = static_cast<uint8_t>(Record->byte_9);

    *reinterpret_cast<uint16_t*>(&self->multiCount_) = 0;

    self->brains_ = byte2 & 0b00000011;
    self->multi_ = (byte2 >> 2) & 0b00000011;

    self->group_[0] = (byte1 & 0x02) != 0;
    self->group_[1] = (byte1 & 0x04) != 0;
    self->group_[2] = (byte1 & 0x08) != 0;
    self->group_[3] = (byte1 & 0x10) != 0;
    self->group_[4] = (byte1 & 0x20) != 0;
    self->group_[5] = (byte1 & 0x40) != 0;

    self->crossFireTarget_ = -1;
    self->level_ = 0;
    self->crossFire_ = (byte1 >> 7) & 0x01;

    self->dragon_ = (byte8 >> 6) & 0x03;

    *reinterpret_cast<uint16_t*>(&self->air_) = 0;

    self->metal_ = byte9 & 0b00000011;
    self->zombi_ = (byte9 >> 6) & 0x03;

    self->roundActionIndex_ = 0;
    self->jouk_ = (byte2 >> 6) & 0x03;
}


void status::HaveBattleStatus::changeMonsterWithoutHpMp(HaveBattleStatus* self, int index)
{
    dq5::level::CharacterType currentType = self->type_;

    if (currentType == dq5::level::CharacterType::MONSTER)
    {
        status::HaveStatusInfo::setupStatus((status::HaveStatusInfo_0*)self->haveStatusInfo_, index, 0);
        currentType = self->type_;  
    }

    self->originalType_ = currentType;

    int oldIndex = self->index_;
    self->type_ = dq5::level::CharacterType::MONSTER;
    self->originalIndex_ = oldIndex;
    self->index_ = index;
}

void status::HaveBattleStatus::changeMonster(HaveBattleStatus* self, int index)
{
    dq5::level::CharacterType currentType = self->type_;

    if (currentType == dq5::level::CharacterType::MONSTER)
    {
        status::HaveStatusInfo::setup((status::HaveStatusInfo_0*)self->haveStatusInfo_, index, 0);
        currentType = self->type_;
    }

    self->originalType_ = currentType;

    int oldIndex = self->index_;
    self->type_ = dq5::level::CharacterType::MONSTER;
    self->originalIndex_ = oldIndex;
    self->index_ = index;
}


void status::HaveBattleStatus::setSelectCommand(HaveBattleStatus* self,status::HaveBattleStatus::SelectCommand command,int index)
{
    self->selectCommand_ = command;
    self->selectIndex_ = index;
}

