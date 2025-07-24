#include "status/CharacterStatus.h"


void status::CharacterStatus::setDamageAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.set(0);
}

bool status::CharacterStatus::isDeathDisable(CharacterStatus* self)
{
    return 0;
}

void status::CharacterStatus::setMosyasRelease(CharacterStatus* self)
{
    
}

void status::CharacterStatus::setMosyasChange(CharacterStatus * self,CharacterStatus* target)
{
    
}


void status::CharacterStatus::setMosyasChange(CharacterStatus* self, int monsterIndex)
{
    
}

bool status::CharacterStatus::isRebirthAnimation(CharacterStatus* self)
{
    return self->damageAnimationFlag_.test(9);
}


void status::CharacterStatus::clearRebirthAnimation(CharacterStatus* self)
{
    self->damageAnimationFlag_.clear(9);
}


void status::CharacterStatus::setRebirthAnimation(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(9);
}


bool status::CharacterStatus::isEscapeAnimation(CharacterStatus* self)
{
    return self->damageAnimationFlag_.test(7);
}


void status::CharacterStatus::clearEscapeAnimation(CharacterStatus* self)
{
    self->damageAnimationFlag_.clear(7);
}

void status::CharacterStatus::setEscapeAnimation(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(7);
}

bool status::CharacterStatus::isMultiDamageAnimation2nd(CharacterStatus* self)
{
    return self->damageAnimationFlag_.test(6);
}


void status::CharacterStatus::clearMultiDamageAnimation2nd(CharacterStatus* self)
{
    self->damageAnimationFlag_.clear(6);
}


void status::CharacterStatus::setMultiDamageAnimation2nd(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(6);
}

bool status::CharacterStatus::isMultiDamageAnimation(CharacterStatus* self)
{
    return self->damageAnimationFlag_.test(5);
}

void status::CharacterStatus::clearMultiDamageAnimation(CharacterStatus* self)
{
    return self->damageAnimationFlag_.clear(5);
}

void status::CharacterStatus::setMultiDamageAnimation(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(5);
}


bool status::CharacterStatus::isMultiDamageAnimation2(CharacterStatus* self)
{
    return self->damageAnimationFlag_.test(4);
}

void status::CharacterStatus::clearMultiDamageAnimation2(CharacterStatus* self)
{
    self->damageAnimationFlag_.clear(4);
}

void status::CharacterStatus::setMultiDamageAnimation2(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(4);
}


bool status::CharacterStatus::isMultiDamageAnimation1(CharacterStatus* self) {
    return self->damageAnimationFlag_.test(3);
}

void status::CharacterStatus::clearMultiDamageAnimation1(CharacterStatus* self)
{
    self->damageAnimationFlag_.clear(3);
}


void status::CharacterStatus::setMultiDamageAnimation1(CharacterStatus* self)
{
    self->damageAnimationFlag_.set(3);
}

bool status::CharacterStatus::isDisappearAnimation(CharacterStatus* self) {
    return self->damageAnimationFlag_.test(2);
}

void status::CharacterStatus::setDisappearAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.set(2);
}

void status::CharacterStatus::clearDisappearAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.clear(2);
}

bool status::CharacterStatus::isDeathAnimation(CharacterStatus* self) {
    return self->damageAnimationFlag_.test(1);
}

void status::CharacterStatus::setDeathAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.set(1);
}

void status::CharacterStatus::clearDeathAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.clear(1);
}


bool status::CharacterStatus::isAddDamageAnimation(CharacterStatus* self) {
    return self->damageAnimationFlag_.test(8); 
}

void status::CharacterStatus::setAddDamageAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.set(8);
}

void status::CharacterStatus::clearAddDamageAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.clear(8);
}

bool status::CharacterStatus::isDamageAnimation(CharacterStatus* self) {
    return self->damageAnimationFlag_.test(0);  
}


void status::CharacterStatus::clearDamageAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.clear(0);
}


void status::CharacterStatus::setNoneAnimation(CharacterStatus* self) {
    self->damageAnimationFlag_.reset();           
    self->damageAnimation_ = status::CharacterStatus::DamageAnimation::NoAnim;              
    self->damageAnimationMulti_ = status::CharacterStatus::DamageAnimation::NoAnim;         
}

void status::CharacterStatus::clearMenuStatusFlag(CharacterStatus* self) {
    self->menuStatusFlag_.reset();
}

bool status::CharacterStatus::isMenuStatusFlag(CharacterStatus* self,status::HaveStatusInfo::DiffStatus status)
{
    return self->menuStatusFlag_.test(static_cast<int>(status));
}




void status::CharacterStatus::clearMenuStatusFlag(CharacterStatus* self, status::HaveStatusInfo::DiffStatus status)
{
    self->menuStatusFlag_.clear(static_cast<int>(status));
}


void status::CharacterStatus::changeMonsterWithEtc(CharacterStatus* self, int index)
{
    status::HaveBattleStatus* p_haveBattleStatus;

    p_haveBattleStatus = &self->haveBattleStatus_;
    status::HaveBattleStatus::changeMonsterWithoutHpMp(&self->haveBattleStatus_, index);
    status::HaveBattleStatus::setupMonster(p_haveBattleStatus);
}

void status::CharacterStatus::changeMonsterWithoutHpMp(CharacterStatus* self, int index)
{
    status::HaveBattleStatus::changeMonsterWithoutHpMp(&self->haveBattleStatus_, index);
}


void status::CharacterStatus::changeMonster(CharacterStatus* self, int index)
{
    status::HaveBattleStatus::changeMonster(&self->haveBattleStatus_, index);
}

void status::CharacterStatus::cleanup(CharacterStatus* self)
{

}


void status::CharacterStatus::setup(CharacterStatus* self, dq5::level::CharacterType type, int group, int index)
{
    if (type == dq5::level::CharacterType::MONSTER)
    {
        status::HaveStatusInfo::setup((status::HaveStatusInfo_0*)&self->haveStatusInfo_, index, 0);
        self->haveStatusInfo_.haveBattleStatus_ = &self->haveBattleStatus_;
        status::HaveBattleStatus::setup(&self->haveBattleStatus_, dq5::level::CharacterType::MONSTER, group, index);
        self->haveBattleStatus_.haveStatusInfo_ = &self->haveStatusInfo_;
    }
    self->menuStatusFlag_.reset();
}

bool status::CharacterStatus::isActionEnableWithoutDragoram(CharacterStatus* self)
{
    return status::HaveStatusInfo::isActionEnableWithoutDragoram((status::HaveStatusInfo_0*)&self->haveStatusInfo_);
}

bool status::CharacterStatus::isActionEnable(CharacterStatus* self)
{
    return status::HaveStatusInfo::isActionEnable((status::HaveStatusInfo_0*)&self->haveStatusInfo_);
}



status::CharacterStatus::CharacterStatus() :

    haveStatusInfo_{},
    haveBattleStatus_{},
    characterIndex_(-1),
    menuStatusFlag_{},
    damageAnimation_(DamageAnimation::NoAnim),
    damageAnimationMulti_(DamageAnimation::NoAnim),
    damageAnimationFlag_{},
    type_{}
{
    menuStatusFlag_.reset();
    damageAnimationFlag_.reset();
    type_.reset();
}

void status::CharacterStatus::printAction(CharacterStatus* self)
{
    
}

void status::CharacterStatus::print(CharacterStatus* self)
{
    
}

void status::CharacterStatus::setMenuStatusFlag(CharacterStatus* self,status::HaveStatusInfo::DiffStatus status)
{
    self->menuStatusFlag_.set(static_cast<int>(status));
}

