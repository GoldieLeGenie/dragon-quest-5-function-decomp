#pragma once

#include "args/DataObject.h"
#include "args/PlacementParameterParam.h"
#include "args/GAME_START_ST.h"
#include "args/fx32.h"

namespace args {
    struct PlacementParameter {
        DataObject dataObject_;
        PlacementParameterParam::Param param_;
    };

}
