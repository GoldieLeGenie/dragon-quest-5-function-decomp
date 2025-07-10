#include "status/UseActionParam.h"


void status::UseActionParam::setActionIndex(UseActionParam* self,int actionIndex) {
	self->actionIndex_ = actionIndex;
}

void status::UseActionParam::setPlayerEffectValue(UseActionParam* self,int value) {
	self->playerEffectValue_ = value < 0 ? 0 : value;
}

void status::UseActionParam::setMonsterEffectValue(UseActionParam* self,int value) {
	self->monsterEffectValue_ = value & ~(value >> 31);
}