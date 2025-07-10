#pragma once
#include <cstring>
#include "ar/Node.h"

namespace ar {

    template <typename T, size_t N>
    struct NodeArray {
        Node<T> array_[N];
        int useIndex_;

        NodeArray() : useIndex_(0) {
            std::memset(array_, 0xFF, sizeof(array_));
        }

        Node<T>& operator[](size_t i) { return array_[i]; }
        const Node<T>& operator[](size_t i) const { return array_[i]; }

        void clear() {
            std::memset(array_, 0xFF, sizeof(array_));
            useIndex_ = 0;
        }

        int allocateNode() {
            if (useIndex_ >= static_cast<int>(N)) return -1;
            return useIndex_++;
        }

        size_t size() const {
            return static_cast<size_t>(useIndex_);
        }
    };

} 
