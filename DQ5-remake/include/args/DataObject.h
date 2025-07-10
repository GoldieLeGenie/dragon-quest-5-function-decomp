#pragma once
#include <cstdint>

namespace args {

    struct DataObject {
        void* m_addr;  // 0x00
        int   m_flag;  // 0x04
    };

} // namespace args
