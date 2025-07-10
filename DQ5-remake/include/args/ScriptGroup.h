#pragma once 
#include "args/DataObject.h"
#include "args/ScriptObject.h"

namespace args {

    struct ScriptGroup {
        DataObject dataObject_;
        ScriptObject mainScriptObject_;
        ScriptObject scriptObject_[48];
        int scriptObjectCount_;
        bool scriptObjectEnableFlag_[48];
    };
}