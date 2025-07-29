#pragma once
#include <cstdint>
#include <cstring>

namespace status {

    struct BaseStatus {
        int32_t exp_;             // 0x00 : Expérience totale
        uint16_t strength_;       // 0x04 : Force
        uint16_t protection_;     // 0x06 : Défense
        uint16_t hp_;             // 0x08 : Points de vie actuels
        uint16_t hpMax_;          // 0x0A : Points de vie max
        uint16_t mp_;             // 0x0C : Points de magie actuels
        uint16_t mpMax_;          // 0x0E : Points de magie max
        uint8_t agility_;         // 0x10 : Agilité
        uint8_t wisdom_;          // 0x11 : Intelligence / sagesse
        uint8_t luck_;            // 0x12 : Chance
        uint8_t level_;           // 0x13 : Niveau actuel
        static void setHpMax(BaseStatus* self, uint16_t hp);
        static void setMpMax(BaseStatus* self,uint16_t mp);
        static void addStrength(BaseStatus* self,char str);
        static void addAgility(BaseStatus* self,char agi);
        static void addProtection(BaseStatus* self, char pr);
        static void addWisdom(BaseStatus* self, char wis);
        static void addHpMax(BaseStatus* self, char hp);
        static void clear(BaseStatus* self);
        static void setStrength(BaseStatus* self, uint8_t  str);
        static void setWisdom(BaseStatus* self, uint8_t wis);
        static void setProtection(BaseStatus* self, uint8_t pr);
        static void setMp(BaseStatus* self, uint16_t mp);
        static void setLuck(BaseStatus* self, uint8_t luck);
        static void setHp(BaseStatus* self, uint16_t hp);
        static void setAgility(BaseStatus* self, uint8_t agi);
        static void addMpMax(BaseStatus* self, int8_t mp);
        static void addLuck(BaseStatus* self, uint8_t lc);
        BaseStatus() {
            std::memset(this, 0, 0x13);
        }
        ~BaseStatus();
        // Taille totale : 0x14 (20 octets)
    };

} // namespace status
