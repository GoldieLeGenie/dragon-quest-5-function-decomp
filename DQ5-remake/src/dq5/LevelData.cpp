#include "dq5/LevelData.h"

namespace dq5::level {
    LevelData levelData;  
}

int dq5::level::LevelData::getLoadType(dq5::level::LevelData* self,int index){
    if (!self->info_.m_addr) return 0;
    auto* data = static_cast<const int32_t*>(self->info_.m_addr);
    return data[index];
}
