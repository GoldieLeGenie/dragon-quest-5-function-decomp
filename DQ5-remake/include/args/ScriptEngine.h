#pragma once 
#include "args/DataObject.h"
#include "args/ScriptGroup.h"


namespace args {
    struct ScriptEngine {
        DataObject dataObject_;       // offset 0x00
        ScriptGroup scriptGroup_;     // offset 0x08
        bool enable_;                 // offset 0x1ED54
        
    };
}