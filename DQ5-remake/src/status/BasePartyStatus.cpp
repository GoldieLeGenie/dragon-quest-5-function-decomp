#include "status/BasePartyStatus.h"
#include "iostream"
#include "status/StageAttribute.h"
#include "status/UseActionFlag.h"






void status::BasePartyStatus::setGold(BasePartyStatus * self,uint32_t gold) {
    self->gold_ = gold;
}

void status::BasePartyStatus::setBankMoney(BasePartyStatus* self, uint32_t money) {
    uint32_t MAX_BANK_MONEY = 99999000;
    self->bankMoney_ = (money < MAX_BANK_MONEY) ? money : MAX_BANK_MONEY;
}

void status::BasePartyStatus::setCasinoCoin(BasePartyStatus* self, uint32_t coin) {
    self->casinoCoin_ = coin;
}

void status::BasePartyStatus::setMedalCoin(BasePartyStatus* self, uint32_t coin){
    self->medalCoin_ = coin;
}

void status::BasePartyStatus::addGold(BasePartyStatus* self, int gold) {
    self->gold_ = gold;
}

void status::BasePartyStatus::addMedalCoin(BasePartyStatus* self, int coin) {
    self->medalCoin_ = coin;
}

bool status::BasePartyStatus::isCarriageEnter(BasePartyStatus* self) {
    return self->carriage_
        && status::StageAttribute::isCarriageEnable((const StageAttribute*)&status::g_StageAttribute)
        && status::StageAttribute::isCarriageEnter((const StageAttribute*)&status::g_StageAttribute);
}


void status::BasePartyStatus::addCasinoCoin(BasePartyStatus* self, int coin) {
    self->casinoCoin_ = coin;
}

status::BasePartyStatus::BasePartyStatus(BasePartyStatus* self) {
    self->carriage_ = 0;

    self->bashaEnable_ = true;

    self->haveItemSack_ = status::HaveItemSack();
}

int status::BasePartyStatus::getBattleExp(BasePartyStatus* self) {
    return self->battleExp_;
}

int status::BasePartyStatus::getBattleGold(BasePartyStatus* self)
{
    return self->battleGold_;
}

void status::BasePartyStatus::reflectBattleGold(BasePartyStatus* self) {
    int gain = self->battleGold_;

    if (status::UseActionFlag::doubleFlag_) {
        gain *= 2;
        self->battleGold_ = gain;
    }

    unsigned int total = self->gold_ + gain;
    self->gold_ = (total < 999999) ? total : 999999;
}

void status::BasePartyStatus::addBankMoney(BasePartyStatus* self,uint32_t money) {
    uint32_t v2;
    uint32_t v3;

    v2 = self->bankMoney_ + money;
    v3 = 999999;
    if (v2 < v3)
        self->bankMoney_ = v2;
    else
        self->bankMoney_ = v3;
}
