#pragma once
#include "args/DataObject.h"
#include "args/ScriptTree.h"

namespace args {

    struct ScriptParam {
        DataObject dataObject_;
        ScriptTree scriptTree_;
        int count_;
        char* tree_;
        int* offset_;
        char* command_;
    };
} 
