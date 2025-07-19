#pragma once
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct SplitMessage {

        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;
    };
}