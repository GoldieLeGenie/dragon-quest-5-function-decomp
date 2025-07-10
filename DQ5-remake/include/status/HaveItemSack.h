#pragma once
#include <cstdint>
#include "status/BaseHaveItem.h"
#include "status/ItemData.h"

#pragma pack(push, 1)

namespace status {

    struct alignas(4) HaveItemSack : public BaseHaveItem {
        enum class SortType : uint32_t {
            None = 0,
            ByType = 1,
            ByName = 2,
            ByQuantity = 3
        };

        SortType sortType_;                  // 0x14
        ItemData itemArray_[274];           // 0x18 → 0x478
        char name_[26];                     // 0x478 → 0x492
        bool cursedName_;                   // 0x492
    };

} 

#pragma pack(pop)