#pragma once
#include "dq5/EncountDarkTile.h"
#include "dq5/EncountFairyTile.h"
#include "dq5/EncountTile3.h"
#include "dq5/EncountTile2.h"
#include "dq5/EncountTile1.h"
#include "dq5/EncountSeaTile2.h"
#include "dq5/EncountSeaTile1.h"
#include "dq5/EncountSeaTile1.h"
#include "args/DataObject.h"

namespace dq5::level {

    struct LevelData {
        args::DataObject info_;
        EncountSeaTile1 encountSeaTile1_;
        EncountSeaTile2 encountSeaTile2_;
        EncountTile1 encountTile1_;
        EncountTile2 encountTile2_;
        EncountTile3 encountTile3_;
        EncountFairyTile encountFairyTile_;
        EncountDarkTile encountDarkTile_;
        static int getLoadType(dq5::level::LevelData* self, int index);
    };
    extern LevelData levelData;
}
