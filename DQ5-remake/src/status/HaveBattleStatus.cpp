#include "status/HaveBattleStatus.h"
#include "status/StageAttribute.h"
#include "ar/rand.h"



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





