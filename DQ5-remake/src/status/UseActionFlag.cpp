#include "status/UseActionFlag.h"

bool status::UseActionFlag::doubleFlag_ = false;  // définition initiale
bool status::UseActionFlag::eventBattle_ = false;  // définition initiale
int status::UseActionFlag::flag_ = 0;
bool status::UseActionFlag::timeReverseFlag_ = false;

void status::UseActionFlag::setZaoraruInvalid(bool flag)
{
    int newFlag = status::UseActionFlag::flag_ & ~0x08;  
    if (flag)
        newFlag |= 0x08;  
    status::UseActionFlag::flag_ = newFlag;
}

