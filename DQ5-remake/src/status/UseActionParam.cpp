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

    std::memset(self, 0, 0x5E); 

    for (int offset = 148; offset < 1012; offset += 48) {
        auto* msg = reinterpret_cast<UseActionMessage*>(reinterpret_cast<char*>(self) + offset);
        status::UseActionMessage::clear(msg);
    }

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
    for (int i = 148; i != 1012; i += 48) {
        status::UseActionMessage::clear(reinterpret_cast<UseActionMessage*>(reinterpret_cast<char*>(this) + i));
    }
    status::UseActionParam::clear(this);
}

