#pragma once
#include <cstdint>

namespace status {

    struct UseActionMessage {
        int beforeMessage_[2];   // 0x00 - 0x08
        int execMessage_[4];     // 0x08 - 0x18
        int spclMessage_[2];     // 0x18 - 0x20
        int resultMessage_[2];   // 0x20 - 0x28
        int addMessage_[2];      // 0x28 - 0x30

        static void clear(UseActionMessage* self);            // Efface tous les champs à zéro
    };

} // namespace status
