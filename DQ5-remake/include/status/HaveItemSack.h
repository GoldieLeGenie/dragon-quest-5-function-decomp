#pragma once
#include <cstdint>
#include "status/BaseHaveItem.h"
#include "status/ItemData.h"
#include "status/haveItem.h"


namespace status {

    struct HaveItemSack : public BaseHaveItem {
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

        static void execThrow(HaveItemSack* self, int index);
        static void fill(HaveItemSack* self);
        static int add(HaveItemSack* self, int itemIndex, int count);
        static bool del(BaseHaveItem* self, int ctrlId);
        static void sortOutItemOne(HaveItemSack* self, status::HaveItem* haveItem);
        static int add(HaveItemSack* self, int itemIndex);
        static void sortOutItem(HaveItemSack* self, status::HaveItem* haveItem);
        static void sortOutSack(HaveItemSack* self, SortType type);
        static void setNameString(HaveItemSack* self, const char* name);
        void operator=(const status::HaveItemSack* rhs);
        static constexpr char defaultName_[10] = {
            static_cast<char>(0xE3), static_cast<char>(0x81), static_cast<char>(0xB5),
            static_cast<char>(0xE3), static_cast<char>(0x81), static_cast<char>(0x8F),
            static_cast<char>(0xE3), static_cast<char>(0x82), static_cast<char>(0x8D),
            '\0'
        };

        HaveItemSack() {
            // Appel implicite au constructeur BaseHaveItem
            item_ = itemArray_;
            itemMax_ = 274;

            // Initialise tous les ItemData
            for (int i = 0; i < 274; ++i) {
                itemArray_[i] = ItemData(); // ou utiliser une méthode d'init
            }

         
            // Copier le nom par défaut (ex : depuis une constante globale)
            std::memcpy(this->name_, defaultName_, sizeof(name_));
        }

        ~HaveItemSack() = default;


    };

} 
