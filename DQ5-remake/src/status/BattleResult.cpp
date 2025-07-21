#include "status/BattleResult.h"
#include "dq5/BookData.h"
#include "dq5/MonsterData.h"

int status::BattleResult::bookDataEntryCount = 0;


bool status::BattleResult::isDisablePlayerDemolition(BattleResult* self)
{
	return self->disablePlayerDemolition_;
}

void status::BattleResult::setDisablePlayerDemolition(BattleResult* self, bool flag)
{
	self->disablePlayerDemolition_ = flag;
}

int status::BattleResult::getMonsterBookIndex(BattleResult* self, int monsterIndex)
{
    if (bookDataEntryCount == 0)
        return -1;

    for (int i = 0; i < static_cast<int>(bookDataEntryCount); ++i)
    {
        void* record = dq5::level::BookData::binary_.getRecord(
            i,
            dq5::level::BookData::addr_,
            dq5::level::BookData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
        );

        auto entryMonsterIndex = *reinterpret_cast<uint16_t*>(record);
        if (entryMonsterIndex == static_cast<uint16_t>(monsterIndex))
            return i;
    }
    return -1;
}


int status::BattleResult::getMonsterFriendly(BattleResult* self, int index)
{
    void* record = dq5::level::BookData::binary_.getRecord(
        index+1,
        dq5::level::BookData::addr_,
        dq5::level::BookData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
    );

    return *reinterpret_cast<unsigned char*>(static_cast<unsigned char*>(record) + 2);
}


int16_t  status::BattleResult::getMonsterItemIndex(BattleResult* self, int index)
{
    void* bookRecord = dq5::level::BookData::binary_.getRecord(
        index + 1,
        dq5::level::BookData::addr_,
        dq5::level::BookData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
    );

    uint16_t monsterIndex = *reinterpret_cast<uint16_t*>(bookRecord);

    auto& resultEntry = self->result_[index];

    if (resultEntry.itemCount_ != 0)
    {
        void* monsterRecord = dq5::level::MonsterData::binary_.getRecord(
            monsterIndex,
            dq5::level::MonsterData::addr_,
            dq5::level::MonsterData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
        );

        return *reinterpret_cast<short*>(reinterpret_cast<char*>(monsterRecord) + 13);
    }

    if (resultEntry.monsterCount_ >= 10)
    {
        void* monsterRecord = dq5::level::MonsterData::binary_.getRecord(
            monsterIndex,
            dq5::level::MonsterData::addr_,
            dq5::level::MonsterData::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
        );

        return *reinterpret_cast<short*>(reinterpret_cast<char*>(monsterRecord) + 13);
    }

    return -1;
}

uint32_t status::BattleResult::getMonsterTotalGold(BattleResult* self, int index)
{
    return status::BattleResult::getMonsterGold(self, index) * self->result_[index].monsterCount_;
}


uint32_t status::BattleResult::getMonsterGold(BattleResult* self, int index)
{
    void* bookRecord = dq5::level::BookData::binary_.getRecord(
        index + 1,
        dq5::level::BookData::addr_,
        dq5::level::BookData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
    );

    uint16_t monsterIndex = *reinterpret_cast<uint16_t*>(bookRecord);

    void* monsterRecord = dq5::level::MonsterData::binary_.getRecord(
        monsterIndex,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
    );

    uint16_t gold = *(reinterpret_cast<uint16_t*>(monsterRecord) + 5);

    return static_cast<uint32_t>(gold);
}


uint32_t status::BattleResult::getMonsterTotalExp(BattleResult* self, int index)
{
    return status::BattleResult::getMonsterExp(self, index) * self->result_[index].monsterCount_;
}


uint32_t status::BattleResult::getMonsterExp(BattleResult* self, int index)
{
    void* bookRecord = dq5::level::BookData::binary_.getRecord(
        index + 1,
        dq5::level::BookData::addr_,
        dq5::level::BookData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
    );

    uint16_t monsterIndex = *reinterpret_cast<uint16_t*>(bookRecord);

    void* monsterRecord = dq5::level::MonsterData::binary_.getRecord(
        monsterIndex,
        dq5::level::MonsterData::addr_,
        dq5::level::MonsterData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::MonsterData::loadSwitch_)
    );

    uint16_t exp = *(reinterpret_cast<uint16_t*>(monsterRecord) + 6);

    return static_cast<uint32_t>(exp);
}

int status::BattleResult::getMonsterEncountCount(BattleResult* self)
{
    int count = 0;

    constexpr int totalBits = 210;
    constexpr int bitsPerInt = 32;
    const uint32_t* bitfield = reinterpret_cast<const uint32_t*>(self) + 303;

    for (int bitIndex = 0; bitIndex < totalBits; ++bitIndex)
    {
        int intIndex = bitIndex / bitsPerInt;  
        int bitPos = bitIndex % bitsPerInt;    

        if (bitfield[intIndex] & (1u << bitPos))
            ++count;
    }

    return count;
}

bool status::BattleResult::isMonsterEncount(const BattleResult* self, uint32_t index)
{
    return self->encountFlag_.flag_[index >> 5].test(index & 0x1F);
}

void status::BattleResult::regenesisMonsterLevel(BattleResult* self, int index, uint8_t level)
{
    status::BattleResult::Result* BattleResult = &self->result_[index];
    uint8_t* p_level = &BattleResult->level_;
    int CurrenLvl = BattleResult->level_;

    if (CurrenLvl == 0)
    {
        if (level < 1)
            level = 1;
        if (level > 99)
            level = 99;

        *p_level = level;
    }
}


int status::BattleResult::getMonsterIndex(BattleResult* self, int index)
{
    void* record = dq5::level::BookData::binary_.getRecord(
        index + 1,
        dq5::level::BookData::addr_,
        dq5::level::BookData::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::BookData::loadSwitch_)
    );

    return static_cast<int>(*reinterpret_cast<const uint16_t*>(record));
}


void status::BattleResult::addMonsterItemCount(BattleResult* self, int index)
{
    uint16_t maxCount = 999;

    BattleResult::Result& result = self->result_[index];

    if (static_cast<uint16_t>(result.itemCount_ + 1) < maxCount)
        result.itemCount_ += 1;
    else
        result.itemCount_ = maxCount;
}

void status::BattleResult::addMonsterCount(BattleResult* self, int index)
{
    uint16_t maxCount = 999;

    BattleResult::Result& result = self->result_[index];

    if (static_cast<uint16_t>(result.monsterCount_ + 1) < maxCount)
        result.monsterCount_ += 1;
    else
        result.monsterCount_ = maxCount;
}


void status::BattleResult::setMonsterEncount(BattleResult* self, uint32_t index, bool flag)
{
    uint32_t* base = reinterpret_cast<uint32_t*>(reinterpret_cast<char*>(self) + 303 * sizeof(uint32_t));
    uint32_t mask = 1u << (index & 0x1F);
    uint32_t& target = base[index >> 5];

    if (flag)
        target |= mask;
    else
        target &= ~mask;
}


void status::BattleResult::initialize(BattleResult* self)
{
    for (uint32_t i = 0; i < 202; ++i)
    {
        self->result_[i].level_ = 0;
        self->result_[i].monsterCount_ = 0;
        self->result_[i].itemCount_ = 0;

        uint32_t flags = self->encountFlag_.flag_[i >> 5].get();
        flags &= ~(1u << (i & 0x1F));
        self->encountFlag_.flag_[i >> 5].setValue(flags);
    }

    self->playerVictory_ = false;
    self->playerDemolition_ = false;
    self->disablePlayerDemolition_ = false;
    self->playerDemolitionMessage_ = 2;
    self->battleTurnCount_ = 0;
    self->battleExecute_ = false;
    self->demolitionPartyTalk_ = false;

    self->firstAttack_ = dq5::level::CharacterType::NONE;
    self->debugFirstAttack_ = dq5::level::CharacterType::NONE;
}

