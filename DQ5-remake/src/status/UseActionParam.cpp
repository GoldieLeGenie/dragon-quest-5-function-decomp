#include "status/UseActionParam.h"
#include <iostream>

void status::UseActionParam::setActionIndex(UseActionParam* self,int actionIndex) {
	self->actionIndex_ = actionIndex;
}

void status::UseActionParam::setPlayerEffectValue(status::UseActionParam* self, int value)
{
    self->playerEffectValue_ = value & ~(value >> 31);
}

void status::UseActionParam::setMonsterEffectValue(status::UseActionParam* self, int value)
{
    self->monsterEffectValue_ = value & ~(value >> 31);
}

void status::UseActionParam::clear(UseActionParam* self)
{
    // Remise à zéro des premiers champs
    self->actorHaveItemSack_ = nullptr;
    self->targetCount_ = 0;
    self->damage_ = 0;
    self->currentTargetIndex_ = 0;

    // Clear brut (offset 0x5E = 94 bytes, mais à éviter en C++ moderne)
    std::memset(self, 0, 0x5E); 

    // Réinitialisation des messages d'action
    for (int offset = 148; offset < 1012; offset += 48) {
        auto* msg = reinterpret_cast<UseActionMessage*>(reinterpret_cast<char*>(self) + offset);
        status::UseActionMessage::clear(msg);
    }

    // Réinitialisation des derniers champs (au-delà de 1012)
    self->result_ = 0;
    self->itemIndex_ = 0;
    self->itemSortIndex_ = 0;
    self->actionDefenceValue_ = 0;
    self->effectValue_ = 0;
    self->playerEffectValue_ = 0;
    self->monsterEffectValue_ = 0;
}



void status::UseActionParam::clearMessage(UseActionParam* self)
{
    constexpr int startOffset = 148;
    constexpr int endOffset = 1012;
    constexpr int step = 48;

    for (int offset = startOffset; offset < endOffset; offset += step) {
        auto* msg = reinterpret_cast<UseActionMessage*>(reinterpret_cast<char*>(self) + offset);
        UseActionMessage::clear(msg);
    }
}


status::UseActionParam::UseActionParam() {
    // Initialisation des UseActionMessage à partir de l'offset 148 jusqu'à 1012 (864 octets => 18 * 48)
    for (int i = 148; i != 1012; i += 48) {
        status::UseActionMessage::clear(reinterpret_cast<UseActionMessage*>(reinterpret_cast<char*>(this) + i));
    }

    // Pas besoin d'appeler explicitement le constructeur de actionFlag_, il est appelé automatiquement

    // Appel de la méthode clear
    status::UseActionParam::clear(this);
}

