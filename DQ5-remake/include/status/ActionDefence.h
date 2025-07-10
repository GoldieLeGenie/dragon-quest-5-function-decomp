#pragma once
#include <cstdint>
#include "dq5/CharacterType.h"

namespace status {

    struct alignas(4) ActionDefence {
        bool disable_;              // 0x00
       
        dq5::level::CharacterType characterType_;  // 0x04

        // Défenses élémentaires et altérations
        uint8_t mera_;              // 0x08
        uint8_t gira_;              // 0x09
        uint8_t io_;                // 0x0A
        uint8_t hyado_;             // 0x0B
        uint8_t bagi_;              // 0x0C
        uint8_t dein_;              // 0x0D
        uint8_t zaki_;              // 0x0E
        uint8_t megante_;           // 0x0F
        uint8_t nifram_;            // 0x10
        uint8_t manusa_;            // 0x11
        uint8_t medapani_;          // 0x12
        uint8_t rariho_;            // 0x13
        uint8_t rarihoma_;          // 0x14
        uint8_t mahotone_;          // 0x15
        uint8_t mahotora_;          // 0x16
        uint8_t rukani_;            // 0x17
        uint8_t rukanan_;           // 0x18
        uint8_t sukara_;            // 0x19

        // Résistances additionnelles
        uint8_t fire_;              // 0x1A
        uint8_t blizzard_;          // 0x1B
        uint8_t poison_;            // 0x1C
        uint8_t venom_;             // 0x1D
        uint8_t spazz_;             // 0x1E
        uint8_t rest_;              // 0x1F

        // Chances de succès
        uint8_t all_;               // 0x20
        uint8_t half_;              // 0x21
        uint8_t success_1_4_;       // 0x22
        uint8_t success_1_6_;       // 0x23
        uint8_t success_1_8_;       // 0x24
        uint8_t success_3_4_;       // 0x25
        uint8_t success_3_8_;       // 0x26
        uint8_t success_5_8_;       // 0x27

        // Statut divers
        uint8_t embrace_;           // 0x28
       
        static void initActionDefence(ActionDefence* self);
        // Taille totale : 0x2C
    };

} // namespace status
