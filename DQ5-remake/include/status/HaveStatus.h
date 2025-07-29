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
        int exp_;                   // 0x08
        uint16_t gold_;                 // 0x0C
        uint16_t jobId_;                // 0x0E
        uint16_t sexId_;                // 0x10

        // Informations générales
        uint8_t iconIndex_;             // 0x12
        uint8_t level_;                 // 0x13
        uint8_t levelMax_;              // 0x14
        char playerType_;             // 0x15
        char isPlayer_;               // 0x16
        char isBattleNpc_;            // 0x17
        char isNoBattleNpc_;          // 0x18
        char isSpecialNpc_;           // 0x19
        char isMonster_;              // 0x1A
        uint8_t monsterId_;             // 0x1B

        // Attributs divers
        int equipAttrIndex_;        // 0x1C
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
        static char seinenki_;
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
        static void addBaseHp(HaveStatus* self, int hp);
        static void setup(HaveStatus* self, uint16_t index, bool flag);
        static void setupPlayer(status::HaveStatus* self, uint16_t index);
        static void setupMonster(HaveStatus* self, uint16_t index, bool flag);
        static void copyHaveStatus(HaveStatus* self, HaveStatus* rhs);
        static void print(HaveStatus* self);
        static uint16_t getDefence(const HaveStatus* self);
        static uint16_t getAttack(const HaveStatus* self);
        static void addMpMax(status::HaveStatus* self, char mp);
        static void setLevelupExp(HaveStatus* self, int val);
        static void addExp(HaveStatus* self, int exp);
        static int getIconIndex(const status::HaveStatus* self);
        static uint16_t levelupAjust(HaveStatus* self, uint16_t value, uint16_t diff, uint16_t normal);
        static uint16_t levelupAjustSpecial(HaveStatus* self, uint16_t value, uint16_t diff, uint16_t normal);
        static uint16_t levelupAdd(HaveStatus* self, uint16_t value);
        static void levelup(HaveStatus* self, bool resetExp);
        static void debugLevelup(HaveStatus* self, int level);
        static bool isLevelup(HaveStatus* self);
        static void resetExpGold(HaveStatus* self);
        static void cleanupPlayer(HaveStatus* self);
        static void cleanup(HaveStatus* self);
        static int getLevelupExp(const status::HaveStatus* self);
        ~HaveStatus();
        HaveStatus();
    };

} // namespace status
