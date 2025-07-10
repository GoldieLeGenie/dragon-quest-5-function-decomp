#include "status/BaseStatus.h"


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