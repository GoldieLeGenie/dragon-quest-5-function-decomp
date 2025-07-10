#pragma once
#include <cstdint>

namespace dq5::level {

    struct PlayerData1 {
        uint32_t exp;              // 0x00 : Expérience nécessaire
        uint16_t strength;         // 0x04
        uint16_t agility;          // 0x06
        uint16_t intelligence;     // 0x08
        uint16_t luck;             // 0x0A
        uint16_t HP;               // 0x0C : Points de vie max
        uint16_t MP;               // 0x0E : Points de magie max

        uint16_t battleAction1;    // 0x10 : Action en combat 1
        uint16_t battleAction2;    // 0x12
        uint16_t battleAction3;    // 0x14
        uint16_t battleAction4;    // 0x16

        uint16_t normalAction1;    // 0x18 : Action hors combat 1
        uint16_t normalAction2;    // 0x1A
        uint16_t normalAction3;    // 0x1C
        uint16_t normalAction4;    // 0x1E

        uint16_t vitality;         // 0x20
        uint8_t level;             // 0x22 : Niveau du personnage
        uint8_t dummy0;            // 0x23 : Alignement/Remplissage
    };

} 
