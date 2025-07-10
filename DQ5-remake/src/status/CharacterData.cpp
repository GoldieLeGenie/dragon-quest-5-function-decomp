#include "dq5/CharacterData.h"
#include "dq5/LevelData.h"
#include "args/ExcelBinaryData.h"
#include "ar/File.h"
#include <cstdio>
#include "dq5/PlayerData1.h"

dq5::level::PlayerData1* dq5::level::CharacterData::getRecord(int index, int level) {
    char buffer[128];

    dq5::level::CharacterData::loadSwitch_ =
        dq5::level::LevelData::getLoadType(reinterpret_cast<dq5::level::LevelData*>(&dq5::level::levelData), 28) != 0;

    std::sprintf(buffer, dq5::level::CharacterData::filename_[0], index);

    dq5::level::CharacterData::binary_.setupHeader(
        buffer,
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterData::loadSwitch_)
    );

    dq5::level::CharacterData::binary_.setup(0x24u);

    void* record = dq5::level::CharacterData::binary_.getRecord(
        level,
        dq5::level::CharacterData::addr_,
        buffer,
        static_cast<ar::File::LoadSwitch>(dq5::level::CharacterData::loadSwitch_)
    );

    return static_cast<dq5::level::PlayerData1*>(record);
}
