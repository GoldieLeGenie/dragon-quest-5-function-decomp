#include "status/ActionDefence.h"
#include "dq5/CharacterType.h"
#include "dq5/MonsterData.h"
#include "iostream"
#include "ar/rand.h"


status::ActionDefence::ActionDefence()
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


int status::ActionDefence::getEffect(ActionDefence* self, dq5::level::ActionParam::DEFENCETYPE type, uint8_t value)
{
   
    uint8_t v = value;

    if (self->characterType_ == dq5::level::CharacterType::MONSTER && self->disable_) {
        if (type <= dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_D) {
            // Probabilité spéciale DEFENCETYPE_TYPE_B ou similaire
            int randVal = ar::rand(1000);
            int result = 0;

            if (randVal < 660 && v != 2)
                result = 1000;
            else if (randVal < 1000 && v < 2)
                result = 1000;

            if (v == 3)
                return result;

            return result;
        }

        if (type == dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_E || type == dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_F) {
            // Type E: simple table
            if (v < 2)
                return 1000;
            if (v == 2)
                return 660;
            if (v == 3)
                return v - 3;  // v == 3 => 0

            return 0;
        }
    }

    if (self->characterType_ == dq5::level::CharacterType::PLAYER && self->disable_) {
        // Mapping linéaire [1–6] → [0–5]
        if (v >= 1 && v <= 6)
            v -= 1;
    }

    switch (type) {
    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_A: {
        int randVal = ar::rand(1000);
        if (v == 0 && randVal < 800) return 1000;
        if (v == 1 && randVal < 330) return 1000;
        if (v == 2 && randVal < 100) return 1000;
        return 0;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_B: {
        int randVal = ar::rand(1000);
        if (v < 2 && randVal < 660) return 1000;
        if (v == 3) return 0;
        return 0;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_C: {
        int randVal = ar::rand(1000);
        if (v == 0 && randVal < 750) return 1000;
        if (v == 1 && randVal < 250) return 1000;
        if (v == 2 && randVal < 100) return 1000;
        return 0;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_D:
        if (v == 0) return 1000;
        if (v == 1) return 800;
        if (v == 2) return 330;
        if (v == 3) return 0;
        return 0;

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_E:
        if (v < 2) return 1000;
        if (v == 2) return 660;
        if (v == 3) return 0;
        return 0;

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_F:
        if (v == 0) return 1000;
        if (v == 1) return 750;
        if (v == 2) return 250;
        if (v == 3) return 100;
        return 0;

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_Z: {
        int randVal = ar::rand(1000);
        const int thresholds[] = { 750, 660, 500, 330, 250, 100 };
        for (int i = 0; i < 6; ++i) {
            if (v == i && randVal < thresholds[i])
                return 1000;
        }
        if (v == 6 && randVal < 100)
            return 1000;
        if (v == 7)
            return 0;
        return 0;
    }

    default:
        return 0;
    }
}


int status::ActionDefence::getEffectAI(ActionDefence* self, dq5::level::ActionParam::DEFENCETYPE type, uint8_t value) {
    int result = 0;

    switch (type) {
    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_A: {
        int rnd = ar::rand(1000);
        if (value == 0)
            result = (rnd < 1000) ? 1000 : 0;
        else if (value == 1)
            result = 800;
        else if (value == 2)
            result = 330;
        else if (value == 3)
            result = value - 3; 
        break;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_B:
    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_E: {
        if (value < 2)
            result = 1000;
        else if (value == 2)
            result = 660;
        else if (value == 3)
            result = value - 3;
        break;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_C: {
        if (value == 0)
            result = 1000;
        else if (value == 1)
            result = 750;
        else if (value == 2)
            result = 250;
        else if (value == 3)
            result = value - 3;
        break;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_D: {
        if (value == 0)
            result = 1000;
        else if (value == 1)
            result = 800;
        else if (value == 2)
            result = 330;
        else if (value == 3)
            result = value - 3;
        break;
    }

    case dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_F: {
        if (value == 0)
            result = 1000;
        else if (value == 1)
            result = 750;
        else if (value == 2)
            result = 250;
        else if (value == 3)
            result = 100;
        break;
    }

    default:
        result = 0;
        break;
    }

    return result;
}



int status::ActionDefence::calc(ActionDefence* self,dq5::level::ActionParam::DEFENCE kind,dq5::level::ActionParam::DEFENCETYPE type,int actionIndex)
{
    if (self->characterType_ == dq5::level::CharacterType::PLAYER) {
        if (self->disable_)
            return 1000;
    }

    uint8_t value = 0;
    switch (kind) {
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MERA:       value = self->mera_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_GIRA:       value = self->gira_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_IO:         value = self->io_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_HYADO:      value = self->hyado_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_BAGI:       value = self->bagi_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_DEIN:       value = self->dein_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_ZAKI:       value = self->zaki_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MEGANTE:    value = self->megante_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_NIFRAM:     value = self->nifram_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MANUSA:     value = self->manusa_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MEDAPANI:   value = self->medapani_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RARIHO:     value = self->rariho_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MAHOTOON:   value = self->mahotone_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MAHOTORA:   value = self->mahotora_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RUKANI:     value = self->rukani_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUKARA:     value = self->sukara_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_FIRE:       value = self->fire_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_BLIZZARD:   value = self->blizzard_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_POISON:     value = self->poison_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_VENOM:      value = self->venom_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_PARALYSIS:  value = self->spazz_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_REST:       value = self->rest_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RARIHOMA:   value = self->rarihoma_; break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RUKANAN:    value = self->rukanan_; break;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_ALL:
        return 1000;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_HALF:
        return ar::rand(1000) < 500 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_4TH:
        return ar::rand(1000) < 250 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_6TH:
        return ar::rand(1000) < 166 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_8TH:
        return ar::rand(1000) < 125 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_3_4:
        return ar::rand(1000) < 750 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_3_8:
        return ar::rand(1000) < 375 ? 1000 : 0;

    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_5_8:
    {
        int randVal = ar::rand(1000);
        if (randVal <= 624)
            return 0x3E8;  
        return 0;
    }

    default:
        return 0;
    }

    return status::ActionDefence::getEffectAI(self, type, value);
}



int status::ActionDefence::calcAI(ActionDefence* self,dq5::level::ActionParam::DEFENCE kind,dq5::level::ActionParam::DEFENCETYPE type,int actionIndex)
{
    uint8_t mera_value;
    int result;

    switch (kind)
    {
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MERA:
        mera_value = self->mera_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_GIRA:
        mera_value = self->gira_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_IO:
        mera_value = self->io_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_HYADO:
        mera_value = self->hyado_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_BAGI:
        mera_value = self->bagi_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_DEIN:
        mera_value = self->dein_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_ZAKI:
        mera_value = self->zaki_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MEGANTE:
        mera_value = self->megante_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_NIFRAM:
        mera_value = self->nifram_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MANUSA:
        mera_value = self->manusa_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MEDAPANI:
        mera_value = self->medapani_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RARIHO:
        mera_value = self->rariho_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MAHOTOON:
        mera_value = self->mahotone_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_MAHOTORA:
        mera_value = self->mahotora_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RUKANI:
        mera_value = self->rukani_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUKARA:
        mera_value = self->sukara_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_FIRE:
        mera_value = self->fire_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_BLIZZARD:
        mera_value = self->blizzard_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_POISON:
        mera_value = self->poison_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_VENOM:
        mera_value = self->venom_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_PARALYSIS:
        mera_value = self->spazz_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_REST:
        mera_value = self->rest_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RARIHOMA:
        mera_value = self->rarihoma_;
        break;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_RUKANAN:
        mera_value = self->rukanan_;
        break;

        // Défenses avec valeurs fixes
    case dq5::level::ActionParam::DEFENCE::DEFENCE_ALL:
        return 1000;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_HALF:
        return 500;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_4TH:
        return 250;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_6TH:
        return 166;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_8TH:
        return 125;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_3_4:
        return 750;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_3_8:
        return 375;
    case dq5::level::ActionParam::DEFENCE::DEFENCE_SUCCESS_5_8:
        return 625;

    default:
        return 0;
    }

    // Appel de la fonction getEffectAI avec la valeur récupérée
    result = status::ActionDefence::getEffectAI(self, type, mera_value);
    return result;
}

status::ActionDefence::~ActionDefence()
{
    
}


int status::ActionDefence::getEffectValue(ActionDefence* self, int actionIndex) {
    // Récupère le pointeur vers l'enregistrement complet
    uint8_t* record = reinterpret_cast<uint8_t*>(dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    ));

    // Récupère le type defence à l'offset 49
    auto defenceType = static_cast<dq5::level::ActionParam::DEFENCE>(record[49]);

    if (self->characterType_ != dq5::level::CharacterType::NONE) {
        auto defType = static_cast<dq5::level::ActionParam::DEFENCETYPE>((record[60] >> 3) & 7);
        return status::ActionDefence::calc(self, defenceType, defType, 0);
    }
    else {
        uint8_t val = record[60] & 7;
        return status::ActionDefence::getEffect(self, dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_Z, val);
    }
}

int status::ActionDefence::getEffectValueAI(ActionDefence* self, int actionIndex) {
    // Récupère le pointeur vers l'enregistrement complet
    uint8_t* record = reinterpret_cast<uint8_t*>(dq5::level::ActionParam::binary_.getRecord(
        actionIndex,
        dq5::level::ActionParam::addr_,
        dq5::level::ActionParam::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::ActionParam::loadSwitch_)
    ));

    // Récupère le type defence à l'offset 49
    auto defenceType = static_cast<dq5::level::ActionParam::DEFENCE>(record[49]);

    if (self->characterType_ != dq5::level::CharacterType::NONE) {
        auto defType = static_cast<dq5::level::ActionParam::DEFENCETYPE>((record[60] >> 3) & 7);
        return status::ActionDefence::calcAI(self, defenceType, defType, 0);
    }
    else {
        uint8_t val = record[60] & 7;
        return status::ActionDefence::getEffect(self, dq5::level::ActionParam::DEFENCETYPE::DEFENCETYPE_TYPE_Z, val);
    }
}

void status::ActionDefence::setupPlayer(status::ActionDefence* self, uint16_t  index)
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

void status::ActionDefence::clear(ActionDefence* self)
{
    self->success_5_8_ = 0;
    self->success_3_4_ = 0;
    memset(&self->mera_, 0, 0x11);
    self->success_1_4_ = 0;
    self->spazz_ = 0;
    self->fire_ = 0;
}

void status::ActionDefence::print(ActionDefence* self)
{
    
}