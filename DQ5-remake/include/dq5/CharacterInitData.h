#pragma once
#include <cstdint>
#include "args/ExcelBinaryData.h"


namespace dq5::level {
    struct CharacterInitData {

        static CharacterInitData* getRecord(int playerIndex, int level);
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;


        uint32_t exp;              // 0x00: Experience
        uint16_t friendID;         // 0x04
        uint16_t index;            // 0x06
        uint16_t strength;         // 0x08
        uint16_t agility;          // 0x0A
        uint16_t vitality;         // 0x0C
        uint16_t intelligence;     // 0x0E
        uint16_t luck;             // 0x10
        uint16_t HP;               // 0x12
        uint16_t MP;               // 0x14
        uint16_t gold;             // 0x16
        uint16_t weapon;           // 0x18
        uint16_t armor;            // 0x1A
        uint16_t shield;           // 0x1C
        uint16_t hat;              // 0x1E
        uint16_t battle1;          // 0x20
        uint16_t battle2;          // 0x22
        uint16_t battle3;          // 0x24
        uint16_t battle9;          // 0x26
        uint16_t normal1;          // 0x28
        uint16_t normal2;          // 0x2A
        uint16_t normal3;          // 0x2C
        uint16_t normal9;          // 0x2E
        uint16_t CGNumber;         // 0x30
        uint16_t job;              // 0x32
        uint16_t sex;              // 0x34
        uint16_t actionMonsterID;  // 0x36
        uint16_t monsterID;        // 0x38
        uint16_t battle4;          // 0x3A
        uint16_t battle5;          // 0x3C
        uint16_t battle6;          // 0x3E
        uint16_t battle7;          // 0x40
        uint16_t battle8;          // 0x42
        uint16_t normal4;          // 0x44
        uint16_t normal5;          // 0x46
        uint16_t normal6;          // 0x48
        uint16_t normal7;          // 0x4A
        uint16_t normal8;          // 0x4C
        uint8_t equipment;         // 0x4E
        uint8_t icon;              // 0x4F
        uint8_t mostLv;            // 0x50
        uint8_t level;             
        uint8_t talk1;             
        uint8_t talk2;             
        int8_t  byte_1;            
        uint8_t dummy0;            
        uint8_t dummy1;            
        uint8_t dummy2;           
    };
} 
