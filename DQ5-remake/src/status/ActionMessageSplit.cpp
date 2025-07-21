#include "status/ActionMessageSplit.h"
#include "dq5/SplitMessage.h"
#include "ar/rand.h"
#include "iostream"


char status::ActionMessageSplit::splitFlag_ = 0;
int status::ActionMessageSplit::actionIndex_ = 0;

int status::ActionMessageSplit::getMessageSpecial(int splitIndex)
{
    void* record = dq5::level::SplitMessage::binary_.getRecord(
        splitIndex,
        dq5::level::SplitMessage::addr_,
        dq5::level::SplitMessage::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::SplitMessage::loadSwitch_)
    );

    status::splitMessage_ = reinterpret_cast<intptr_t>(record);

    int result = *reinterpret_cast<int*>(static_cast<uint8_t*>(record) + 216);

    if (result != 0)
        status::ActionMessageSplit::splitFlag_ = 1;

    return result;
}


int status::ActionMessageSplit::getMessageZero(int splitIndex)
{
    void* record = dq5::level::SplitMessage::binary_.getRecord(
        splitIndex,
        dq5::level::SplitMessage::addr_,
        dq5::level::SplitMessage::filename_[0],
        static_cast<ar::File::LoadSwitch>(dq5::level::SplitMessage::loadSwitch_)
    );

    return 0;
}


int status::ActionMessageSplit::getMessageRandom(int splitIndex)
{
    int actionIndex = status::ActionMessageSplit::actionIndex_;

    if (actionIndex == 340 || actionIndex == 381 || actionIndex == 349)
    {
        auto* record = static_cast<std::uint32_t*>(dq5::level::SplitMessage::binary_.getRecord(
            splitIndex,
            dq5::level::SplitMessage::addr_,
            dq5::level::SplitMessage::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::SplitMessage::loadSwitch_)
        ));

        status::splitMessage_ = reinterpret_cast<std::intptr_t>(record);

        std::uint32_t candidates[11];
        std::memset(candidates, 0, sizeof(candidates));
        int count = 0;

        for (int i = 42; i <= 52; ++i) {
            if (record[i] != 0) {
                candidates[count++] = record[i];
            }
        }

        if (count > 0) {
            status::ActionMessageSplit::splitFlag_ = 0;
            return candidates[ar::rand(count)];
        }

        return 0;
    }

    return 0;
}


int status::ActionMessageSplit::getMessageNoTarget(int splitIndex)
{
    int result = 0;

    if (status::ActionMessageSplit::actionIndex_ == 132)
    {
        void* record = dq5::level::SplitMessage::binary_.getRecord(
            splitIndex,
            dq5::level::SplitMessage::addr_,
            dq5::level::SplitMessage::filename_[0],
            static_cast<ar::File::LoadSwitch>(dq5::level::SplitMessage::loadSwitch_)
        );

        status::splitMessage_ = reinterpret_cast<std::intptr_t>(record);

        int value = *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(record) + 164);
        if (value != 0)
        {
            result = value;
            status::ActionMessageSplit::splitFlag_ = 0;
        }
    }

    return result;
}


