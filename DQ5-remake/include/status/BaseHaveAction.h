#pragma once

#include <cstdint>

namespace status {

    struct BaseHaveAction {
        uint16_t battleAction_;   // 0x00
        uint16_t usuallyAction_;  // 0x02
        uint8_t actionLevel_;     // 0x04
        uint8_t actionFlag_;      // 0x05
        static void clear(BaseHaveAction* self);
        static bool isEnable(BaseHaveAction* self);
        static void add(BaseHaveAction* self, int battleAction, int usuallyAction);
        static void remembering(BaseHaveAction* self);
        static bool isRemembering(BaseHaveAction* self);
        static void clearRemembering(BaseHaveAction* self);
        static int getAction(BaseHaveAction* self);
        static int getUsuallyAction(BaseHaveAction* self);
        static bool isRemember(BaseHaveAction* self);
        static void del(BaseHaveAction* self);
        ~BaseHaveAction();


    };

} // namespace status
