#pragma once
#include <cstdint>
#include "args/ExcelBinaryData.h"


namespace dq5::level {

    struct AppriseItem {

        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;
    };
}