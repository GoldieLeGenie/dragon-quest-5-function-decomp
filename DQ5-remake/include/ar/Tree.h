#pragma once
#include "ar/NodeArray.h"

namespace ar {

    template <typename T, size_t N>
    struct Tree {
        int (**_vptr$Tree)(void);  // vtable (peut rester nullptr)
        int rootNodeIndex;
        int currentNodeIndex;
        int level;
        NodeArray<T, N> Nodes;

        Tree()
            : _vptr$Tree(nullptr),
            rootNodeIndex(-1),
            currentNodeIndex(-1),
            level(0) {}
    };

} 
