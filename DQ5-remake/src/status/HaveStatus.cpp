#include <cstdint>
#include <iostream>
#include "status/HaveStatus.h"
#include "status/StageAttribute.h"
#include "status/HaveBattleStatus.h"
#include "dq5/ActionParam.h"
#include "dq5/CharacterType.h"
#include "dq5/CommandType.h"
#include "dq5/dq5sex.h"
#include "dq5/CharacterData.h"
#include "dq5/PlayerData1.h"
#include "ar/rand.h"


bool status::HaveStatus::seinenki_ = false;

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



