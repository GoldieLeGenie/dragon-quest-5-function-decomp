#include "status/UseActionFlag.h"
#include <cstdint>
#include <iostream>


bool status::UseActionFlag::doubleFlag_;  // définition initiale
bool status::UseActionFlag::eventBattle_;  // définition initiale
int status::UseActionFlag::flag_;
int status::UseActionFlag::callFriendMonsterIndex_;
bool status::UseActionFlag::timeReverseFlag_;
int status::UseActionFlag::workParam_;

uint16_t status::UseActionFlag::execActionCount_[502] = {};

void status::UseActionFlag::setZaoraruInvalid(bool flag)
{
    int newFlag = status::UseActionFlag::flag_ & ~0x08;  
    if (flag)
        newFlag |= 0x08;  
    status::UseActionFlag::flag_ = newFlag;
}

int status::UseActionFlag::getExecActionCount(int actionIndex) {
    return status::UseActionFlag::execActionCount_[actionIndex];
}


void status::UseActionFlag::addExecActionCount(int actionIndex) {
    ++status::UseActionFlag::execActionCount_[actionIndex];
}



int status::UseActionFlag::getCallFriendMonsterIndex()
{
    return status::UseActionFlag::callFriendMonsterIndex_;
}


int status::UseActionFlag::getCallFriendMonsterGroup()
{
    return status::UseActionFlag::workParam_;
}


bool status::UseActionFlag::isCallFriend()
{
    return (static_cast<uint8_t>(status::UseActionFlag::flag_) >> 2) & 0x1;
}

void status::UseActionFlag::setCallFriend(bool flag, int monsterGroup, int monsterIndex)
{
    // Mise à jour des paramètres
    status::UseActionFlag::workParam_ = monsterGroup;
    status::UseActionFlag::callFriendMonsterIndex_ = monsterIndex;

    // Manipulation du bit 2 (valeur 0x4)
    constexpr int CALL_FRIEND_BIT_MASK = 0x4;

    if (flag)
        status::UseActionFlag::flag_ |= CALL_FRIEND_BIT_MASK;   
    else
        status::UseActionFlag::flag_ &= ~CALL_FRIEND_BIT_MASK;  
}


bool status::UseActionFlag::isZaoraruInvalid()
{
    return (static_cast<uint8_t>(status::UseActionFlag::flag_) >> 3) & 0x1;
}


int status::UseActionFlag::isFailedNoUseMp()
{
    return  (static_cast<uint8_t>(status::UseActionFlag::flag_) >> 1) & 1;
}

void status::UseActionFlag::setFailedNoUseMp(bool flag)
{
    int newFlag = status::UseActionFlag::flag_ & ~0x02; 
    if (flag)
        newFlag |= 0x02;
    status::UseActionFlag::flag_ = newFlag;
}

int status::UseActionFlag::isBreakPrayRing()
{
    return status::UseActionFlag::flag_ & 1;
}

void status::UseActionFlag::setBreakPrayRing(bool flag)
{
    int newFlag = status::UseActionFlag::flag_ & ~0x01;
    if (flag)
        newFlag |= 0x01;
    status::UseActionFlag::flag_ = newFlag;
}

void status::UseActionFlag::clearExecActionCount()
{
    std::memset(status::UseActionFlag::execActionCount_, 0, sizeof(status::UseActionFlag::execActionCount_));
}

void status::UseActionFlag::clear()
{
    status::UseActionFlag::flag_ = 0;
    std::memset(status::UseActionFlag::execActionCount_, 0, sizeof(status::UseActionFlag::execActionCount_));
}

status::UseActionFlag::UseActionFlag()
{
}