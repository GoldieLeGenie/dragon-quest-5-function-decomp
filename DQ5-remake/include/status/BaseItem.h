#pragma once 

namespace status {
    struct BaseItem {
        int itemIndex_;
        bool available_;
        virtual ~BaseItem() = default;
    };

    struct ExtraItemDokukesisou : public BaseItem {};
    struct ExtraItemYakusou : public BaseItem {};
}