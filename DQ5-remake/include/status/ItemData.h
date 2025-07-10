#pragma once
#include "ar/BitFlag.h"

namespace status {

    struct ItemData {
        int index_;
        uint8_t count_;
        ar::Flag8 flag_;

        ItemData() : index_(0), count_(0), flag_() {}

        ItemData(int index, uint8_t count, ar::Flag8 flag)
            : index_(index), count_(count), flag_(flag) {}
    };
} 