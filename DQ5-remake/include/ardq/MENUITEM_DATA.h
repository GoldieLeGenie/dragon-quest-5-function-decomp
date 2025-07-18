#pragma once
#include "iostream"

namespace ardq {
    struct MENUITEM_DATA {
        uint8_t code;  
        uint8_t view;
        int16_t x;
        int16_t y;
        int16_t w;
        int16_t h;
    };
}