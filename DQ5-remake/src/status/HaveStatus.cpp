#include <cstdint>
#include <iostream>
#include "status/HaveStatus.h"
#include "status/StageAttribute.h"
#include "status/HaveBattleStatus.h"
#include "dq5/ActionParam.h"
#include "dq5/CharacterType.h"
#include "dq5/MonsterData.h"
#include "dq5/CommandType.h"
#include "dq5/dq5sex.h"
#include "dq5/CharacterData.h"
#include "dq5/PlayerData1.h"
#include "status/PlayerData.h"
#include "ar/rand.h"


char status::HaveStatus::seinenki_;

uint16_t status::HaveStatus::getHp(const HaveStatus* self) {
    uint16_t hp = self->baseStatus_.hp_;
    return (hp < 9999) ? hp : 9999;
}

void status::HaveStatus::addBaseMp(HaveStatus* self,int mp) {
    int currentMp = self->baseStatus_.mp_;

    if (currentMp != 1000) {
        int mpMax = self->baseStatus_.mpMax_;
        int added = currentMp + mp;

        // Clamp à 0 si négatif
        if (added < 0) added = 0;

        // Ne pas dépasser mpMax
        if (added < mpMax) {
            self->baseStatus_.mp_ = added;
        }
        else {
            self->baseStatus_.mp_ = mpMax;
        }
    }
}

void status::HaveStatus::setMp(HaveStatus* self,uint16_t mp) {
    uint16_t mpMax = self->baseStatus_.mpMax_;
    if (mp < mpMax)
        mpMax = mp;
    self->baseStatus_.mp_ = mpMax;
}


void status::HaveStatus::setHp(HaveStatus* self, uint16_t hp) {
    uint16_t hpMax = self->baseStatus_.hpMax_;
    if (hp < hpMax)
        hpMax = hp;
    self->baseStatus_.hp_ = hpMax;
}

uint16_t status::HaveStatus::getMp(const HaveStatus* self)
{
    uint16_t mp = self->baseStatus_.mp_;
    return (mp < 999) ? mp : 999;
}


uint16_t status::HaveStatus::getHpMax(const HaveStatus* self)  {
    uint16_t hpMax = self->baseStatus_.hpMax_;
    return (hpMax < 9999) ? hpMax : 9999;
}

uint16_t status::HaveStatus::getMpMax(const HaveStatus* self) {
    uint16_t mpMax = self->baseStatus_.mpMax_;
    return (mpMax < 999) ? mpMax : 999;
}


uint8_t status::HaveStatus::getAgility(const HaveStatus* self) {
    return self->baseStatus_.agility_;
}

uint8_t status::HaveStatus::getWisdom(const HaveStatus* self) {
    return self->baseStatus_.wisdom_;
}

uint8_t status::HaveStatus::getLuck(const HaveStatus* self) {
    return self->baseStatus_.luck_;
}

void status::HaveStatus::setHpMax(HaveStatus* self,uint16_t hp) {
    status::BaseStatus::setHpMax(&self->baseStatus_, hp);

    uint16_t currentHp = self->baseStatus_.hp_;

    if (currentHp <= 1)
        currentHp = 1;

    if (currentHp >= self->baseStatus_.hpMax_)
        currentHp = self->baseStatus_.hpMax_;

    self->baseStatus_.hp_ = currentHp;
}


void status::HaveStatus::setMpMax(HaveStatus* self, uint16_t mp) {
    status::BaseStatus::setMpMax(&self->baseStatus_, mp);

    uint16_t currentMp = self->baseStatus_.mp_;
    uint16_t maxMp = self->baseStatus_.mpMax_;

    if (currentMp < maxMp)
        maxMp = currentMp;

    self->baseStatus_.mp_ = maxMp;
}


dq5::Sex status::HaveStatus::getSex(const HaveStatus* self) {
    if (self->sexId_ == 277)
        return dq5::Sex::SEX_FEMALE;
    if (self->sexId_ == 276)
        return dq5::Sex::SEX_MALE;
    return dq5::Sex::SEX_NONE;
}


int status::HaveStatus::getCharaIndex(const status::HaveStatus* self) {
    if (self->playerIndex_ != 1)
        return self->charaIndex_;

    if (!status::HaveStatus::seinenki_)
        return self->charaIndex_;

    return 201;
}


void status::HaveStatus::setCharaIndex(HaveStatus* self,int index)
{
    self->charaIndex_ = index;
}

void status::HaveStatus::addStrength(HaveStatus* self,char str) {
    status::BaseStatus::addStrength(&self->baseStatus_, str);
}

void status::HaveStatus::addAgility(HaveStatus* self, char str)
{
    status::BaseStatus::addAgility(&self->baseStatus_, str);
}

void status::HaveStatus::addProtection(HaveStatus* self, char str)
{
    status::BaseStatus::addProtection(&self->baseStatus_,str);
}

void status::HaveStatus::addWisdom(HaveStatus* self, char str)
{
    status::BaseStatus::addWisdom(&self->baseStatus_, str);
}

void status::HaveStatus::addLuck(HaveStatus* self, char lk) {
    int sum = self->baseStatus_.luck_ + lk;

    // Clamp à 0 si négatif
    if (sum < 0)
        sum = 0;

    // Clamp à 255 si dépassement
    if (sum >= 255)
        sum = 255;

    self->baseStatus_.luck_ = static_cast<uint8_t>(sum);
}

void status::HaveStatus::addHpMax(HaveStatus* self,char hp) {
    if (self->baseStatus_.mp_ != 1000) {
        status::BaseStatus::addHpMax(&self->baseStatus_, hp);
    }
}

dq5::level::PlayerData1* status::HaveStatus::getLevelupData(int playerIndex, int level) {
    // Cas spéciaux : pas de données de level-up pour les ID 8 et 15
    if (playerIndex == 8 || playerIndex == 15) {
        return nullptr;
    }

    // Redirection spéciale : playerIndex 7 utilise les données du 6
    if (playerIndex == 7) {
        playerIndex = 6;
    }

    // Appel à la fonction centrale pour récupérer les données de level-up
    return dq5::level::CharacterData::getRecord(playerIndex, level);
}


void status::HaveStatus::setLevel(HaveStatus* self,uint8_t lvl) {
    // Ne s'applique qu'à certains types de personnages
    uint8_t playerType = self->playerType_;
    if (playerType > 5)
        return;

    // Le masque 0x26 correspond aux types : 1, 2, 5 (car 0x26 = 0b100110)
    if (((1 << playerType) & 0x26) == 0)
        return;

    int playerIndex = self->playerIndex_;
    dq5::level::PlayerData1* record = nullptr;

    // Pas de données de level-up pour les index 8 et 15
    if (playerIndex != 8 && playerIndex != 15) {
        if (playerIndex == 7)
            playerIndex = 6;

        // Le jeu demande level + 1
        record = dq5::level::CharacterData::getRecord(playerIndex, lvl + 1);
    }

    // Applique le niveau et l’expérience correspondante (si dispo)
    self->level_ = lvl;
    if (record)
        self->baseStatus_.exp_ = record->exp;
}


void status::HaveStatus::setExp(HaveStatus* self, int exp) {
    // Si l'EXP n'est pas désactivée (ex: personnage temporaire ?)
    if (self->exp_ == -1)
        return;

    // Clamp à 0 si négatif
    if (exp < 0)
        exp = 0;

    // Clamp supérieur à 0xFFFFFF (16 777 215)
    if (exp > 0xFFFFFF)
        exp = 0xFFFFFF;

    self->exp_ = exp;
}


void status::HaveStatus::addBaseHp(HaveStatus* self, int hp) {
    int hpMax = self->baseStatus_.hpMax_;
    int sum = self->baseStatus_.hp_ + hp;

    int clamped = sum & ~(sum >> 31);

    if (clamped < hpMax)
        hpMax = clamped;

    self->baseStatus_.hp_ = hpMax;
}

void status::HaveStatus::setup(HaveStatus* self, uint16_t index, bool flag)
{
    if (flag)
        status::HaveStatus::setupPlayer(self, index);
    else
        status::HaveStatus::setupMonster(self, index, 1);
}

void status::HaveStatus::setupPlayer(status::HaveStatus* self, uint16_t index) {
    char* rawData = reinterpret_cast<char*>(&status::PlayerDataAll::playerData_);
    status::PlayerData* player = reinterpret_cast<status::PlayerData*>(rawData + 276 * index);

    self->playerIndex_ = index;
    self->playerKindIndex_ = player->kindIndex_;

    self->baseStatus_.strength_ = static_cast<uint8_t>(player->baseStatus_.strength_);
    self->baseStatus_.agility_ = player->baseStatus_.agility_;
    self->baseStatus_.protection_ = static_cast<uint8_t>(player->baseStatus_.protection_);
    self->baseStatus_.wisdom_ = player->baseStatus_.wisdom_;
    self->baseStatus_.luck_ = player->baseStatus_.luck_;
    self->baseStatus_.hp_ = player->baseStatus_.hp_;
    self->baseStatus_.hpMax_ = player->baseStatus_.hpMax_;
    self->baseStatus_.mp_ = player->baseStatus_.mp_;
    self->baseStatus_.mpMax_ = player->baseStatus_.mpMax_;

    self->charaIndex_ = status::PlayerData::getCgIndex(player);
    self->monsterIndex_ = status::PlayerData::getActionMonsterIndex(player);
    self->iconIndex_ = status::PlayerData::getIconIndex(player);
    self->equipAttrIndex_ = status::PlayerData::getEquipAttrIndex(player);

    self->exp_ = player->baseStatus_.exp_;
    self->level_ = player->baseStatus_.level_;
    self->levelMax_ = status::PlayerData::getLevelMax(player);
    self->jobId_ = status::PlayerData::getJob(player);
    self->monsterId_ = status::PlayerData::getMonsterIndex(player);
    self->sexId_ = status::PlayerData::getSexId(player);
    self->command_ = player->command_;
    self->actionCursorIndex_ = player->actionCursorIndex_;

    // Si le niveau actuel est inférieur au max, on récupère l'exp du prochain niveau
    if (self->level_ < self->levelMax_) {
        int kind = self->playerKindIndex_;
        dq5::level::PlayerData1* record = nullptr;

        if (kind != 15 && kind != 8) {
            if (kind == 7)
                kind = 6;
            record = dq5::level::CharacterData::getRecord(kind, self->level_ + 1);
        }

        if (record)
            self->baseStatus_.exp_ = record->exp;
    }

    // Détermination du type de personnage
    switch (status::PlayerData::getCharacterKind(player)) {
    case 1: self->playerType_ = 1; break;
    case 2: self->playerType_ = 2; break;
    case 3: self->playerType_ = 3; break;
    case 4:
        self->playerType_ = 4;
        [[fallthrough]];
    case 5: self->playerType_ = 5; break;
    default:
        if (self->playerType_ == 4) {
            *reinterpret_cast<uint32_t*>(&self->baseStatus_.hp_) = 0x000A000A;
        }
        break;
    }
}

void status::HaveStatus::setupMonster(HaveStatus* self, uint16_t index, bool flag) {
    uint16_t* record = reinterpret_cast<uint16_t*>(dq5::level::MonsterData::binary_.getRecord(
        index,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_))
        );
    

    self->baseStatus_.strength_ = record[3];
    self->baseStatus_.agility_ = *(reinterpret_cast<uint8_t*>(record) + 31);
    self->baseStatus_.protection_ = record[4];
    self->baseStatus_.wisdom_ = 0;

    uint16_t hpMax = record[1];
    self->baseStatus_.hpMax_ = hpMax;
    self->baseStatus_.hp_ = hpMax;

    int mpMax = record[2];
    self->baseStatus_.mpMax_ = mpMax;
    self->baseStatus_.mp_ = mpMax;

    if (mpMax == 255) {
        *reinterpret_cast<uint32_t*>(&self->baseStatus_.mp_) = 65537000;
    }

    if (flag) {
        self->playerKindIndex_ = index;
        self->playerIndex_ = index;
        self->charaIndex_ = 0;
        self->iconIndex_ = 0;
        self->exp_ = record[6];
        self->gold_ = record[5];
        self->playerType_ = 6;
        self->isMonster_ = false;
        self->isPlayer_ = false;
    }
}


void status::HaveStatus::copyHaveStatus(HaveStatus* self, HaveStatus* rhs) {
    // Copie directe des membres du sous-struct baseStatus_
    self->baseStatus_.strength_ = rhs->baseStatus_.strength_;
    self->baseStatus_.agility_ = rhs->baseStatus_.agility_;
    self->baseStatus_.wisdom_ = rhs->baseStatus_.wisdom_;
    self->baseStatus_.hp_ = rhs->baseStatus_.hp_;
    self->baseStatus_.mp_ = rhs->baseStatus_.mp_;
    self->baseStatus_.exp_ = rhs->baseStatus_.exp_;

    // Copie des autres membres
    self->exp_ = rhs->exp_;
    self->gold_ = rhs->gold_;
    self->level_ = rhs->level_;
    self->levelMax_ = rhs->levelMax_;
}


void status::HaveStatus::print(HaveStatus* self)
{
    
}

uint16_t status::HaveStatus::getDefence(const HaveStatus* self){
    uint32_t  protection; 
    uint16_t  maxdef;

    protection = self->baseStatus_.protection_;
    maxdef = 9999;
    if (protection < 9999)
        return protection;
    return maxdef;
}

uint16_t status::HaveStatus::getAttack(const HaveStatus* self)
{
    uint32_t strength;
    uint16_t maxatk; 

    strength = self->baseStatus_.strength_;
    maxatk = 9999;
    if (strength < 9999)
        return strength;
    return maxatk;
}

void status::HaveStatus::addMpMax(status::HaveStatus* self, char mp)
{
    status::BaseStatus::addMpMax(&self->baseStatus_, mp);
}


void status::HaveStatus::setLevelupExp(HaveStatus* self, int val)
{
    self->baseStatus_.exp_ = self->exp_ + val;
}


void status::HaveStatus::addExp(HaveStatus* self, int exp) {
    int currentExp = self->exp_;

    if (currentExp != -1) {
        int newExp = currentExp + exp;

        int clampedExp = newExp & ~(newExp >> 31);

        if (clampedExp > 0xFFFFFF) {
            clampedExp = 0xFFFFFF;
        }

        self->exp_ = clampedExp;
    }
}



int status::HaveStatus::getIconIndex(const status::HaveStatus* self) {
    if (self->playerIndex_ != 1) {
        return self->iconIndex_;
    }
    if (!status::HaveStatus::seinenki_) {
        return self->iconIndex_;
    }
    return 2;
}


uint16_t status::HaveStatus::levelupAjust(HaveStatus* self, uint16_t value, uint16_t diff, uint16_t normal) {
    // Constante hex pour division rapide, multipliée par (75 * normal)
    int64_t multiplier = 0x51EB851FLL;
    int64_t mul_result = multiplier * static_cast<int64_t>(75) * static_cast<int64_t>(normal);

    uint32_t limit = (150 * normal) / 100;

    if (limit >= value && (diff + value) <= limit) {
        uint32_t high = static_cast<uint32_t>(mul_result >> 32);
        uint32_t shifted = high >> 5;

        uint32_t low = static_cast<uint32_t>(mul_result);
        if (low < shifted) {
            // Remplace la partie basse par shifted (shifted <= 32 bits)
            mul_result = (mul_result & 0xFFFFFFFF00000000LL) | shifted;
        }
        return static_cast<uint16_t>(mul_result);
    }
    else {
        return static_cast<uint16_t>(ar::rand(2) + value);
    }
}

uint16_t status::HaveStatus::levelupAjustSpecial(HaveStatus* self,uint16_t value,uint16_t diff,uint16_t normal)
{
    uint32_t currentValue = value;

    constexpr int64_t multiplier = 0x51EB851FLL;

    int64_t mulResult = multiplier * static_cast<int64_t>(130 * normal);

    int limit86Percent = static_cast<int>(86 * normal / 100);

    int normalMinus30 = static_cast<int16_t>(normal - 30);

    int clampedNormalMinus30 = normalMinus30 & ~(normalMinus30 >> 31);
    int clampedLimit86Percent = limit86Percent & ~(limit86Percent >> 31);

    
    const int maxLimit = 1000;
    if (clampedNormalMinus30 > maxLimit)
        clampedNormalMinus30 = maxLimit;
    if (clampedLimit86Percent < maxLimit)
        clampedNormalMinus30 = (clampedNormalMinus30 > clampedLimit86Percent) ? clampedNormalMinus30 : clampedLimit86Percent;

    uint32_t mulResultHigh = static_cast<uint32_t>(mulResult >> 32);

    int adjustedHigh = (mulResultHigh >> 5) + 15;

    uint16_t normalPlus60 = normal + 60;
    uint16_t finalLimit = (adjustedHigh < normalPlus60) ? adjustedHigh : normalPlus60;

    if (finalLimit >= currentValue && (diff + currentValue) <= finalLimit) {
        int proposedValue = diff + currentValue;
        if (proposedValue < clampedNormalMinus30) {
            return static_cast<uint16_t>(clampedNormalMinus30);
        }
        return static_cast<uint16_t>(proposedValue);
    }
    else {
        return static_cast<uint16_t>(ar::rand(2) + currentValue);
    }
}

uint16_t status::HaveStatus::levelupAdd(HaveStatus* self, uint16_t value) {
    uint16_t randPart = static_cast<uint16_t>(ar::rand(50) + 75);
    int64_t mulResult = 1374389535LL * static_cast<uint16_t>(randPart * value + 50);

    uint32_t highShifted = (static_cast<uint32_t>(mulResult >> 32)) >> 5;

    if (value <= highShifted) {
        return static_cast<uint16_t>(highShifted);
    }
    return value;
}


void status::HaveStatus::levelup(status::HaveStatus* self, bool flag)
{
    if (self->level_ < self->levelMax_)
    {
        self->level_++;

        if (flag)
            self->exp_ = self->baseStatus_.exp_;

        status::BaseStatus diffStats;  

        int playerKindIndex = self->playerKindIndex_;
        unsigned int level = self->level_;

        
        dq5::level::PlayerData1* record = nullptr;
        if (playerKindIndex != 15 && playerKindIndex != 8)
        {
            if (playerKindIndex == 7)
                playerKindIndex = 6;

            record = dq5::level::CharacterData::getRecord(playerKindIndex, level - 1);
        }

        
        auto vitality = record ? record->vitality : 0;
        auto MP = record ? record->MP : 0;
        auto luck = record ? record->luck : 0;
        auto intelligence = record ? record->intelligence : 0;
        auto agility = record ? record->agility : 0;
        auto HP = record ? record->HP : 0;
        auto strength = record ? record->strength : 0;

        dq5::level::PlayerData1* nextRecord = nullptr;
        if (playerKindIndex != 15 && playerKindIndex != 8)
        {
            if (playerKindIndex == 7)
                playerKindIndex = 6;

            nextRecord = dq5::level::CharacterData::getRecord(playerKindIndex, level);
        }

        if (nextRecord)
        {
            diffStats.agility_ = nextRecord->agility - agility;
            diffStats.wisdom_ = nextRecord->intelligence - intelligence;
            diffStats.luck_ = nextRecord->luck - luck;
            diffStats.protection_ = nextRecord->vitality - vitality;  
            diffStats.hpMax_ = static_cast<uint8_t>(nextRecord->HP - HP);
            diffStats.mpMax_ = static_cast<uint8_t>(nextRecord->MP - MP);
            diffStats.strength_ = static_cast<uint8_t>(nextRecord->strength - strength);
        }
        else
        {
            memset(&diffStats, 0, sizeof(diffStats));
        }

        if (level < self->levelMax_)
        {
            int nextLevelIndex = level + 1;
            int pkIndex = self->playerKindIndex_;
            if (pkIndex == 7)
                pkIndex = 6;

            auto nextNextRecord = dq5::level::CharacterData::getRecord(pkIndex, nextLevelIndex);
            if (nextNextRecord)
            {
                self->baseStatus_.exp_ = nextNextRecord->exp; 
            }
        }

        self->baseStatus_.strength_ = status::HaveStatus::levelupAjustSpecial(self, self->baseStatus_.strength_, diffStats.strength_, nextRecord ? nextRecord->strength : 0);
        self->baseStatus_.agility_ = status::HaveStatus::levelupAjust(self, self->baseStatus_.agility_, diffStats.agility_, nextRecord ? nextRecord->agility : 0);
        self->baseStatus_.protection_ = status::HaveStatus::levelupAjust(self, self->baseStatus_.protection_, diffStats.protection_, vitality);
        self->baseStatus_.wisdom_ = status::HaveStatus::levelupAjust(self, self->baseStatus_.wisdom_, diffStats.wisdom_, intelligence);
        self->baseStatus_.luck_ = status::HaveStatus::levelupAjust(self, self->baseStatus_.luck_, diffStats.luck_, luck);
        self->baseStatus_.hpMax_ = status::HaveStatus::levelupAjustSpecial(self, self->baseStatus_.hpMax_, diffStats.hpMax_, HP);
        self->baseStatus_.mpMax_ = status::HaveStatus::levelupAjust(self, self->baseStatus_.mpMax_, diffStats.mpMax_, MP);

        if (flag)
        {
            self->baseStatus_.hp_ = self->baseStatus_.hpMax_;
            self->baseStatus_.mp_ = self->baseStatus_.mpMax_;
        }

    }
}



void status::HaveStatus::debugLevelup(status::HaveStatus* self, int level)
{
    int levelMax = self->levelMax_;
    if (levelMax == 0)
        return;

    int cappedLevel = levelMax;
    if (level < levelMax)
        cappedLevel = level;

    int playerKindIndex = self->playerKindIndex_;
    dq5::level::PlayerData1* Record = nullptr;

    bool isSpecialKind = (playerKindIndex == 15 || playerKindIndex == 8);
    if (!isSpecialKind)
    {
        if (playerKindIndex == 7)
            playerKindIndex = 6;

        Record = dq5::level::CharacterData::getRecord(playerKindIndex, cappedLevel);
        levelMax = self->levelMax_;
    }

    if (Record)
    {
        int exp = Record->exp;
        uint16_t MP = Record->MP;
        uint16_t HP = Record->HP;
        uint16_t luck = Record->luck;
        uint16_t intelligence = Record->intelligence;
        uint16_t vitality_low = Record->vitality;
        uint16_t strength_low = Record->strength;

        self->baseStatus_.agility_ = Record->agility;
        self->baseStatus_.strength_ = strength_low;
        self->baseStatus_.protection_ = vitality_low;
        self->baseStatus_.wisdom_ = intelligence;
        self->baseStatus_.luck_ = luck;
        self->baseStatus_.hp_ = HP;
        self->baseStatus_.hpMax_ = HP;
        self->baseStatus_.mp_ = MP;
        self->baseStatus_.mpMax_ = MP;
        self->level_ = cappedLevel;
        self->exp_ = exp;

        if (cappedLevel < levelMax)
        {
            int nextPlayerKindIndex = self->playerKindIndex_;
            dq5::level::PlayerData1* nextRecord = nullptr;
            bool nextIsSpecial = (nextPlayerKindIndex == 15 || nextPlayerKindIndex == 8);

            if (!nextIsSpecial)
            {
                if (nextPlayerKindIndex == 7)
                    nextPlayerKindIndex = 6;

                nextRecord = dq5::level::CharacterData::getRecord(nextPlayerKindIndex, cappedLevel + 1);
            }

            if (nextRecord)
                self->baseStatus_.exp_ = nextRecord->exp;
        }
    }
}


bool status::HaveStatus::isLevelup(HaveStatus* self)
{
    bool canLevelUp = false;
    uint32_t  playerType = static_cast<uint8_t>(self->playerType_);
    bool isBlocked;

    if (playerType <= 5)
    {
        isBlocked = ((1 << playerType) & 0x26) == 0;
        if (((1 << playerType) & 0x26) != 0)
            isBlocked = self->level_ == self->levelMax_;
        if (!isBlocked)
            return self->exp_ >= self->baseStatus_.exp_;
    }
    return canLevelUp;
}

void status::HaveStatus::resetExpGold(HaveStatus* self)
{
    uint16_t* record = reinterpret_cast<uint16_t*>(dq5::level::MonsterData::binary_.getRecord(
        self->playerIndex_,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_))
        );

    self->exp_ = record[6];
    self->gold_ = record[5];
}

void status::HaveStatus::cleanupPlayer(HaveStatus* self)
{
    char* playerDataBase = reinterpret_cast<char*>(&status::PlayerDataAll::playerData_);
    char* playerDataPtr = playerDataBase + 276 * self->playerIndex_;

    status::BaseStatus* baseStatusPtr = reinterpret_cast<status::BaseStatus*>(playerDataPtr);

    status::BaseStatus::setStrength(baseStatusPtr + 2, self->baseStatus_.strength_);
    status::BaseStatus::setAgility(baseStatusPtr + 2, self->baseStatus_.agility_);
    status::BaseStatus::setProtection(baseStatusPtr + 2, self->baseStatus_.protection_);
    status::BaseStatus::setWisdom(baseStatusPtr + 2, self->baseStatus_.wisdom_);
    status::BaseStatus::setLuck(baseStatusPtr + 2, self->baseStatus_.luck_);
    status::BaseStatus::setHp(baseStatusPtr + 2, self->baseStatus_.hp_);
    status::BaseStatus::setHpMax(baseStatusPtr + 2, self->baseStatus_.hpMax_);
    status::BaseStatus::setMp(baseStatusPtr + 2, self->baseStatus_.mp_);
    status::BaseStatus::setMpMax(baseStatusPtr + 2, self->baseStatus_.mpMax_);

    (baseStatusPtr + 2)->exp_ = self->exp_;
    (baseStatusPtr + 2)->level_ = self->level_;
    (baseStatusPtr + 1)->agility_ = self->command_;
    (baseStatusPtr + 1)->wisdom_ = self->actionCursorIndex_;
}

void status::HaveStatus::cleanup(HaveStatus* self)
{
    if (self->playerType_ != 6)
        status::HaveStatus::cleanupPlayer(self);
}

status::HaveStatus::~HaveStatus()
{
    this->baseStatus_.~BaseStatus();

}

status::HaveStatus::HaveStatus()
{
    this->baseStatus_ = BaseStatus(); 
}


int status::HaveStatus::getLevelupExp(const status::HaveStatus* self)
{
    return self->baseStatus_.exp_ - self->exp_;
}