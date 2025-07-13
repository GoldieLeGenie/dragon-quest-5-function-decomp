#pragma once
#include <cstdint>
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct MonsterData {

        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;

        uint16_t index;         // 0x00
        uint16_t HP;            // 0x02
        uint16_t MP;            // 0x04
        uint16_t attack;        // 0x06
        uint16_t defence;       // 0x08
        uint16_t money;         // 0x0A
        uint16_t exp;           // 0x0C
        uint16_t action1;       // 0x0E
        uint16_t action2;       // 0x10
        uint16_t action3;       // 0x12
        uint16_t action4;       // 0x14
        uint16_t action5;       // 0x16
        uint16_t action6;       // 0x18
        uint16_t item;          // 0x1A
        uint16_t animIndex;     // 0x1C
        uint8_t  party;         // 0x1E
        uint8_t  agility;       // 0x1F
        uint8_t  ratio;         // 0x20
        uint8_t  animAll;       // 0x21
        uint8_t  anim1;         // 0x22
        uint8_t  anim2;         // 0x23
        uint8_t  anim3;         // 0x24
        uint8_t  anim4;         // 0x25
        uint8_t  anim5;         // 0x26
        uint8_t  anim6;         // 0x27
        int8_t   byte_1;        // 0x28
        int8_t   byte_2;        // 0x29
        int8_t   byte_3;        // 0x2A
        int8_t   byte_4;        // 0x2B
        int8_t   byte_5;        // 0x2C
        int8_t   byte_6;        // 0x2D
        int8_t   byte_7;        // 0x2E
        int8_t   byte_8;        // 0x2F
        int8_t   byte_9;        // 0x30
        int8_t   byte_10;       // 0x31
        int8_t   byte_11;       // 0x32
        uint8_t  dummy0;        // 0x33
       
    };
    extern MonsterData* monsterData2_;
}