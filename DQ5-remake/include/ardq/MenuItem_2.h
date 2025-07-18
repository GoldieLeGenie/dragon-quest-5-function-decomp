#pragma once
#include "iostream"
#include "ardq/MenuItem.h"
#include "ardq/MENUITEM_DATA.h"

namespace ardq {

    struct MenuItem_2 {
        ardq::MenuItem::MENUITEM_TYPE mtype_;       // 0x00
        ardq::MenuItem::CURSORTYPE ctype_;          // 0x04
        bool enablePad_;                      // 0x08
        bool enableCancel_;                   // 0x09
        bool enableDirectButton_;             // 0x0A
        bool enableLoopEdge_;                 // 0x0B
        bool enableSE_;                      // 0x0C
        ardq::MENUITEM_DATA* menuitem_data_;
        int menuitem_temp_x_;                 // 0x14
        int menuitem_temp_y_;                 // 0x18
        int menuitem_width_;                  // 0x1C
        int menuitem_height_;                 // 0x20
        int menuitem_min_;                    // 0x24
        int menuitem_max_;                    // 0x28
        int active_;                         // 0x2C
        ardq::MenuItem::MenuItem::MENUITEM_RESULT lastresult_;  // 0x30
        ardq::MenuItem::MenuItem::MENUITEM_RESULT result_;      // 0x34
        ardq::MenuItem::MenuItem::MENUITEM_REASON reason_;       // 0x38
        MenuPort* m_port;                     // 0x3C
        short m_unityButton;                  // 0x40
        short m_unityPadPress;                // 0x42
        short m_unityPadToggle;               // 0x44
        bool m_bNewType;                     // 0x46
        short m_bExecInput2;                  // 0x48
        void (*m_unityOtherReceive)(const short*); // 0x4C
        short m_portType;                    // 0x50
        bool m_bTermNotClose;                // 0x52
    };

}