#pragma once
#include <cstdint>
#include "status/BaseStatus.h"
#include "dq5/dq5sex.h"
#include "dq5/PlayerData1.h"

namespace status {

    struct HaveStatus {
        // Identifiants et indices
        uint16_t playerIndex_;          // 0x00
        uint16_t playerKindIndex_;      // 0x02
        uint16_t charaIndex_;           // 0x04
        uint16_t monsterIndex_;         // 0x06

        // Valeurs de progression
        int32_t exp_;                   // 0x08
        uint16_t gold_;                 // 0x0C
        uint16_t jobId_;                // 0x0E
        uint16_t sexId_;                // 0x10

        // Informations générales
        uint8_t iconIndex_;             // 0x12
        uint8_t level_;                 // 0x13
        uint8_t levelMax_;              // 0x14
        int8_t playerType_;             // 0x15
        int8_t isPlayer_;               // 0x16
        int8_t isBattleNpc_;            // 0x17
        int8_t isNoBattleNpc_;          // 0x18
        int8_t isSpecialNpc_;           // 0x19
        int8_t isMonster_;              // 0x1A
        uint8_t monsterId_;             // 0x1B

        // Attributs divers
        int32_t equipAttrIndex_;        // 0x1C
        uint8_t command_;               // 0x20
        uint8_t actionCursorIndex_;     // 0x21

        // Statistiques de base
        BaseStatus baseStatus_;  
        
        static uint16_t getHp(const HaveStatus* self);
        static void addBaseMp(HaveStatus* self, int mp);
        static void setMp(HaveStatus* self, uint16_t mp);
        static void setHp(HaveStatus* self, uint16_t hp);
        static uint16_t getHpMax(const HaveStatus* self);
        static uint16_t getMpMax(const HaveStatus* self);
        static uint8_t getAgility(const HaveStatus* self);
        static uint8_t getWisdom(const HaveStatus* self);
        static void setHpMax(HaveStatus* self, uint16_t hp);
        static uint8_t getLuck(const HaveStatus* self) ;
        static void setMpMax(HaveStatus* self,uint16_t mp);
        static dq5::Sex getSex(const HaveStatus* self);
        static int getCharaIndex(const HaveStatus* self);
        static bool seinenki_;
        static void addStrength(HaveStatus* self, char str);
        static void setCharaIndex(HaveStatus* self, int index);
        static void addAgility(HaveStatus* self, char str);
        static void addProtection(HaveStatus* self, char str);
        static void addWisdom(HaveStatus* self, char str);
        static void addLuck(HaveStatus* self, char lk);
        static void addHpMax(HaveStatus* self, char hp);
        static dq5::level::PlayerData1* getLevelupData(int playerIndex, int level);
        static void setLevel(HaveStatus* self, uint8_t lvl);
        static void setExp(HaveStatus* self, int exp);
        static uint16_t getMp(const HaveStatus* self);
    };

} // namespace status
