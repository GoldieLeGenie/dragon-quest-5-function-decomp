#pragma once 
#include <cstdint>
#include "args/ExcelBinaryData.h"
namespace dq5::level {
    struct ShopData3 {
        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;

        uint32_t price;
        uint16_t item;
        uint8_t  dummy0;
        uint8_t  dummy1;
    };
}