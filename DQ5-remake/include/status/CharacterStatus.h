#pragma once

#include "status/HaveStatusInfo_0.h"
#include "status/HaveBattleStatus.h"
#include "dq5/CharacterType.h"
#include "ar/BitFlag.h"

namespace status {

    struct CharacterStatus {
        // Vtable (pour les méthodes virtuelles)
        void** _vptr$CharacterStatus;

        // Type de personnage (joueur, monstre, etc.)
        dq5::level::CharacterType characterType_;           // 0x04

        // Infos de statut global (objets, compétences, etc.)
        HaveStatusInfo haveStatusInfo_;              // 0x08

        // Infos spécifiques au combat
        HaveBattleStatus haveBattleStatus_;          // 0x42C

        // Identifiants divers
        int characterIndex_;                         // 0x4DC
        int characterGroup_;                         // 0x4E0
        int arrayIndex_;                             // 0x4E4

        // Flags
        ar::Flag32 menuStatusFlag_;                    // 0x4E8

        // Animations de dégâts (probablement des enums ou structs légers)
        enum class DamageAnimation : int32_t {
            NoAnim = 0x0,
            Damage = 0x1,
            Death = 0x2,
            Disappear = 0x3,
            MultiDamage = 0x4
        };

        enum class DamageSound : int32_t {
            NoSe = 0x0,
            KaishinSe = 0x1,
            TsukonSe = 0x2
        };

        DamageAnimation damageAnimation_;            // 0x4EC
        DamageAnimation damageAnimationMulti_;       // 0x4F0
        DamageSound damageSound_;                    // 0x4F4

        // Flags liés à l'animation
        ar::Flag32 damageAnimationFlag_;               // 0x4F8
        ar::Flag32 type_;                              // 0x4FC
    };

} // namespace status
