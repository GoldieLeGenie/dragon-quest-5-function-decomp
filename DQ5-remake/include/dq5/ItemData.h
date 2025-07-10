#pragma once
#include "args/ExcelBinaryData.h"

namespace dq5::level {

    struct ItemData {
        unsigned long casino;
        unsigned long menuMes;
        unsigned long meisanhinMsg;
        unsigned long commentMsg;
        unsigned short buyPrice;
        unsigned short sellPrice;
        unsigned short samall;
        unsigned short effect;
        unsigned short action;
        unsigned short battleAction;
        unsigned short typeSort;
        unsigned short nameSort;
        unsigned char equipType;
        unsigned char commentID;
        unsigned char commentType;
        unsigned char type;
        char byte_1;
        char byte_2;
        char byte_3;
        char byte_4;
        char byte_5;
        unsigned char dummy0;
        unsigned char dummy1;
        unsigned char dummy2;

        static args::ExcelBinaryData binary_;
        static void* addr_;
        static const char* filename_[];
        static int loadSwitch_;
    };

} 
