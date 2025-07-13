#pragma once
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct ActionTypeList {
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;

        uint8_t turn;       // 0x00
        uint8_t typeID;     // 0x01
        uint8_t pattern;    // 0x02
        int8_t  byte_1;     // 0x03
        int8_t  byte_2;     // 0x04
        uint8_t dummy0;     // 0x05
        uint8_t dummy1;     // 0x06
        uint8_t dummy2;     // 0x07

    };

}
