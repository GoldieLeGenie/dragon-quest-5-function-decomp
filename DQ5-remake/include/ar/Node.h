#pragma once
#include <cstdint>

namespace ar {

    template <typename T>
    struct Node {
        T value_;             // valeur contenue
        uint8_t parentIndex_; // index du parent
        uint8_t childIndex_;  // index du premier enfant
        uint8_t nextIndex_;   // frère suivant
        uint8_t prevIndex_;   // frère précédent

        Node()
            : value_(0), parentIndex_(0xFF), childIndex_(0xFF),
            nextIndex_(0xFF), prevIndex_(0xFF) {}
    };

} // namespace ar
