#pragma once
#include <cstdint>
#include "ar/BitFlag.h"
#include "dq5/CharacterType.h"
#include "status/HaveStatusInfo.h"

namespace status {
    extern int parupunteEffects[25];
    extern bool isParupunteCarriageEnabled;
    extern int parupunteEventEffects[10];

    struct HaveBattleStatus {
        int groupIndex_;                       // 0x00
        int index_;                            // 0x04
        int originalIndex_;                    // 0x08
        dq5::level::CharacterType originalType_;      // 0x0C
        dq5::level::CharacterType type_;              // 0x10
        HaveStatusInfo* haveStatusInfo_; // 0x14
        int exp_;                              // 0x18
        uint16_t hp_;                          // 0x1C
        uint16_t hpMax_;                       // 0x1E
        uint16_t mp_;                          // 0x20
        uint16_t mpMax_;                       // 0x22
        uint16_t strength_;                    // 0x24
        uint16_t protection_;                  // 0x26
        uint16_t agility_;                     // 0x28
        uint16_t wisdom_;                      // 0x2A
        uint8_t luck_;                         // 0x2C

        int texp_;                             // 0x30
        int tlevel_;                           // 0x34
        bool change_;                          // 0x38

        int actionIndex_;                      // 0x3C
        int actionCount_;                      // 0x40
        int patternIndex_;                     // 0x44
        ar::Flag32 patternFailedFlag_;           // 0x48
        ar::Flag32 disablePattern_;              // 0x4C
        ar::Flag32 disablePattern2nd_;           // 0x50

        enum class SelectCommand :int32_t
        {                                       // XREF: status::HaveBattleStatus/r
                                                 // status::HaveBattleStatus_0/r
            Attack = 0x0,
            Defence = 0x1,
            UseAction = 0x2,
            UseItem = 0x3,
            NoSelect = 0x4,
            AISelect = 0x5,
        };

        int selectIndex_;                      // 0x58
        int selectedGroup_;                    // 0x5C
        int selectedTarget_;                   // 0x60
        int sortIndex_;                        // 0x64
        int selectItemIndex_;                  // 0x68
        int monsterCtrlId_;                    // 0x6C
        bool escape_;                          // 0x70
        uint8_t brains_;                       // 0x71
        uint8_t group_[6];                     // 0x72
        uint8_t crossFire_;                    // 0x78

        int crossFireTarget_;                  // 0x7C
        uint8_t multi_;                        // 0x80
        int8_t multiCount_;                    // 0x81
        int8_t multiCount2_;                   // 0x82
        int8_t multiTurnCount_;                // 0x83
        int8_t multiGlobalCount_;              // 0x84
        int8_t multiExecCount_;                // 0x85
        uint8_t level_;                        // 0x86
        uint8_t dragon_;                       // 0x87
        uint8_t metal_;                        // 0x88
        uint8_t zombi_;                        // 0x89
        uint8_t air_;                          // 0x8A
        uint8_t slime_;                        // 0x8B
        uint8_t jouk_;                         // 0x8C

        int monsterIndexForNpc_;               // 0x90
        int roundActionIndex_;                 // 0x94
        uint8_t mosyasAction_[17];             // 0x98


        int mosyasActionCount_;                // 0xAC
        /*void setupParupunteAction();*/
        static int getParupunteAction(HaveBattleStatus*, bool event);
        static bool eventFlag_;
    };
} 
