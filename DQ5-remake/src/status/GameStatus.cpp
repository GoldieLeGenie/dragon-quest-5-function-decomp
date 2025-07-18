#include "iostream"
#include "status/GameStatus.h"
#include "ar/rand.h"

void status::GameStatus::addPlayTime(GameStatus* self, uint32_t time)
{

    uint32_t CurrentPlayTime = self->playTime_ + time;

    uint32_t MaxPlayTime = 0xCDFD7F0;
    if (CurrentPlayTime < 0xCDFD7F0)
        MaxPlayTime = CurrentPlayTime;
    self->playTime_ = MaxPlayTime;
}

void status::GameStatus::initialize(GameStatus* self)
{
    self->playTime_ = 0;
}

uint32_t status::GameStatus::getPlayTime(GameStatus* self)
{
    return self->playTime_;
}

void status::GameStatus::setPlayTime(GameStatus* self, uint32_t time)
{
    uint32_t MaxPlayTime = 0xCDFD7F0;
    if (time < 0xCDFD7F0)
        MaxPlayTime = time;
    self->playTime_ = MaxPlayTime;
}

void status::GameStatus::resetUniqueID(GameStatus* self)
{
    self->uniqueID_ = ar::rand(256);
}


uint32_t status::GameStatus::getUniqueID(GameStatus* self)
{
    uint32_t GenUniqueId;

    GenUniqueId = self->uniqueID_;
    if (!GenUniqueId)
    {
        GenUniqueId = ar::rand(256);
        self->uniqueID_ = GenUniqueId;
    }
    return GenUniqueId;
}

void status::GameStatus::setUniqueID(GameStatus* self, uint32_t id)
{
    self->uniqueID_ = id;
}

status::GameStatus::GameStatus() {
    this->playTime_ = 0;
}

status::GameStatus::~GameStatus()
{
    
}