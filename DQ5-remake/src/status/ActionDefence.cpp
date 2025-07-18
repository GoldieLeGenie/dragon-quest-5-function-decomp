#include "status/ActionDefence.h"
#include "dq5/CharacterType.h"
#include "dq5/MonsterData.h"
#include "iostream"


void status::ActionDefence::initActionDefence(ActionDefence* self)
{
}

void status::ActionDefence::setup(ActionDefence* self, uint16_t index, dq5::level::CharacterType type)
{
    self->disable_ = 0;
    if (type == dq5::level::CharacterType::MONSTER)
    {
        status::ActionDefence::setupMonster(self, index);
    }
    else
    {
        self->megante_ = 768;
        self->rariho_ = 0;
        self->zaki_ = 0;
        self->bagi_ = 0;
        self->characterType_ = dq5::level::CharacterType::PLAYER;
        self->mera_ = 0;
        self->mahotone_ = 196608;
        self->success_3_4_ = 0;
        self->success_5_8_ = 0;
        self->success_1_4_ = 0;
        self->spazz_ = 0;
        self->fire_ = 0;
    }
}


void status::ActionDefence::setupMonster(ActionDefence* self, uint16_t index)
{
    uint32_t v4; 
    uint32_t v5; 
    uint32_t v6; 
    uint32_t v7; 
    uint32_t v8; 
    uint32_t v9; 
    uint32_t v10; 

    self->characterType_ = dq5::level::CharacterType::MONSTER;

    uint8_t* record = reinterpret_cast<uint8_t*>(dq5::level::MonsterData::binary_.getRecord(
        index,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_))
        );

    v4 = record[43];
    self->io_ = v4 & 3;
    v5 = record[42];
    self->dein_ = v4 >> 6;
    self->bagi_ = (v4 >> 4) & 3;
    self->hyado_ = (v4 >> 2) & 3;
    self->mera_ = (v5 >> 4) & 3;
    self->gira_ = v5 >> 6;
    v6 = record[44];
    self->manusa_ = v6 & 3;
    v5 = record[49];
    self->rarihoma_ = (v6 >> 2) & 3;
    self->zaki_ = (v6 >> 4) & 3;
    self->nifram_ = v6 >> 6;
    self->rariho_ = v5 & 3;
    v7 = record[45];
    self->mahotora_ = v7 & 3;
    self->mahotone_ = (v7 >> 2) & 3;
    self->medapani_ = (v7 >> 4) & 3;
    self->rukani_ = v7 >> 6;
    v8 = record[46];
    self->rukanan_ = v8 & 3;
    self->megante_ = (v8 >> 2) & 3;
    self->poison_ = v8 >> 6;
    v9 = record[47];
    self->venom_ = v9 & 3;
    self->rest_ = (v9 >> 4) & 3;
    v10 = record[48];
    self->spazz_ = (v9 >> 2) & 3;
    self->embrace_ = (v8 >> 4) & 3;
    self->blizzard_ = (v10 >> 2) & 3;
    self->fire_ = (v10 >> 4) & 3;
}

