#include "status/BaseStatus.h"
#include "iostream"

void status::BaseStatus::setHpMax(BaseStatus* self,uint16_t hp) {
    uint16_t maxAllowed = 9999;
    if (hp < maxAllowed)
        maxAllowed = hp;
    self->hpMax_ = maxAllowed;
}


void status::BaseStatus::setMpMax(status::BaseStatus* self, uint16_t mp) {
    uint16_t max = 999;
    if (mp < 999)
        max = mp;
    self->mpMax_ = max;
}

void status::BaseStatus::addStrength(BaseStatus* self, char str) {
    int sum = self->strength_ + str;

    // Clamp inférieur à 0
    if (sum < 0)
        sum = 0;

    // Clamp supérieur à 255
    if (sum >= 255)
        sum = 255;

    self->strength_ = static_cast<uint8_t>(sum);
}

void status::BaseStatus::addAgility(BaseStatus* self, char agi) {
    int sum = self->agility_ + agi;

    if (sum < 0)
        sum = 0;

    if (sum >= 255)
        sum = 255;

    self->agility_ = static_cast<uint8_t>(sum);
}


void status::BaseStatus::addProtection(BaseStatus* self, char pr) {
    int sum = self->protection_ + pr;

    // Clamp à 0 si négatif
    if (sum < 0)
        sum = 0;

    // Clamp à 255 si dépassement
    if (sum >= 255)
        sum = 255;

    self->protection_ = static_cast<uint8_t>(sum);
}


void status::BaseStatus::addWisdom(BaseStatus* self, char wis) {
    int sum = self->wisdom_ + wis;

    // Clamp à 0 si négatif
    if (sum < 0)
        sum = 0;

    // Clamp à 255 si dépassement
    if (sum >= 255)
        sum = 255;

    self->wisdom_ = static_cast<uint8_t>(sum);
}

void status::BaseStatus::addHpMax(BaseStatus* self, char hp) {
    int sum = self->hpMax_ + hp;

    // Clamp inférieur à 0
    if (sum < 0)
        sum = 0;

    // Clamp supérieur à 9999
    if (sum > 9999)
        sum = 9999;

    self->hpMax_ = static_cast<uint16_t>(sum);
}

void status::BaseStatus::clear(BaseStatus* self) {
    std::memset(self, 0, 0x13); 
}


void status::BaseStatus::setStrength(BaseStatus* self, uint8_t  str)
{
    self->strength_ = str;
}

void status::BaseStatus::setWisdom(BaseStatus* self, uint8_t wis)
{
    self->wisdom_ = wis;
}

void status::BaseStatus::setProtection(BaseStatus* self, uint8_t pr)
{
    self->protection_ = pr;
}

void status::BaseStatus::setMp(BaseStatus* self, uint16_t mp) {
    uint16_t maxMP = 999;
    if (mp < 999)
        maxMP = mp;
    self->mp_ = maxMP;
}


void status::BaseStatus::setLuck(BaseStatus* self, uint8_t luck)
{
    self->luck_ = luck;
}

void status::BaseStatus::setHp(BaseStatus* self, uint16_t hp)
{
    uint16_t maxAllowedHp = 9999;
    if (hp < maxAllowedHp)
        maxAllowedHp = hp;
    self->hp_ = maxAllowedHp;
}

void status::BaseStatus::setAgility(BaseStatus* self, uint8_t agi)
{
    self->agility_ = agi;
}

void status::BaseStatus::addMpMax(BaseStatus* self,int8_t mp) {
    int result = static_cast<int>(self->mpMax_) + mp;

    if (result < 0) result = 0;
    if (result > 999) result = 999;

    self->mpMax_ = static_cast<uint16_t>(result);
}


void status::BaseStatus::addLuck(BaseStatus* self, uint8_t lc) {
    int result = static_cast<int>(self->luck_) + lc;

    if (result > 255) result = 255;

    self->luck_ = static_cast<uint8_t>(result);
}


status::BaseStatus::~BaseStatus() {
    
}