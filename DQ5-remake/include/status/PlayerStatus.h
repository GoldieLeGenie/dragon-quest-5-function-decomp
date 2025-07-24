#pragma once
#include "status/CharacterStatus.h"
#include "dq5/StageDamage.h"

namespace status {

    struct PlayerStatus : public CharacterStatus {
        static void setWalkDamage(PlayerStatus* self, int dmg);
        static bool walkDamage(PlayerStatus* self, dq5::StageDamage damage);
        static void walkEnvioronment(PlayerStatus* self);
        static bool setWalkEffectPoison(PlayerStatus* self);
        static bool walkNormal(PlayerStatus* self);
        static bool setWalkEffectVenom(PlayerStatus* self);
        static void setWalkEffectLuckHat(PlayerStatus* self);
        static void setWalkEffectLifeRing(PlayerStatus* self);
        static bool setWalkEffectCurse4(PlayerStatus* self);
        static bool setWalkEffectCurse5(PlayerStatus* self);
        static void setup(PlayerStatus* self, int index);
        static void levelup(PlayerStatus* self, int level);
        static void cleanup(PlayerStatus* self);
        static void clearWalkDamage(PlayerStatus* self, bool flag);
        static void setBestCondition(PlayerStatus* self);
        static void execStartOfRound(PlayerStatus* self);
        static void printAction(status::PlayerStatus* self);
        static void print(PlayerStatus* self);
        static void addItem(PlayerStatus* self, int itemIndex);
        static void delItem(PlayerStatus* self, int index);
        ~PlayerStatus() override = default;
        PlayerStatus();

        short index_;
        short characterIndex_;
        short walkCount_;
        short walkCountBarrier_;
        short walkCountPoison_;
        short walkCountVenom_;
        short walkCountLuckHat_;
        short walkCountLifeRing_;
        short walkCountCurse4_;
        short walkCountCurse5_;
    };

}