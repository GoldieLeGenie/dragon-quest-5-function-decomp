#pragma once
#include <cstdint>
#include "status/HaveItemSack.h"

namespace status {

    struct BasePartyStatus {
        bool bashaEnter_;
        bool bashaEnable_;
        bool fukuro_;
  

        int battleMonsterCount_;
        int battleExp_;
        int battleGold_;

        bool carriage_;
        

        uint32_t gold_;
        uint32_t bankMoney_;
        uint32_t casinoCoin_;
        uint32_t medalCoin_;

        HaveItemSack haveItemSack_;

        static void setGold(BasePartyStatus* self,uint32_t gold);
        static void setBankMoney(BasePartyStatus* self, uint32_t money);
        static void setCasinoCoin(BasePartyStatus* self, uint32_t coin);
        static void setMedalCoin(BasePartyStatus* self, uint32_t coin);
        static void addGold(BasePartyStatus* self, int gold);
        static void addMedalCoin(BasePartyStatus* self, int coin);
        static bool isCarriageEnter(BasePartyStatus* self);
        static int getBattleExp(BasePartyStatus* self);
        static int getBattleGold(BasePartyStatus* self);
        static void reflectBattleGold(BasePartyStatus* self);
        static void addBankMoney(BasePartyStatus* self, uint32_t money);
        static void addCasinoCoin(BasePartyStatus* self, int coin);
        BasePartyStatus(BasePartyStatus* self);
    };

} 
