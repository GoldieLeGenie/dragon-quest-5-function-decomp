// StadiumData.h
#pragma once

#include <cstdint>

struct StadiumData {
    int32_t betOnIndex_;
    uint32_t betCoin_;
    bool stadiumFlag_;
    bool monsterDeath_;
    uint16_t odds_;
    uint16_t monsterID_;
    uint8_t monsterArray_;
    char monsterNumber_;
    uint8_t result_;
    bool doubleUp_;
    bool retire_;
};
