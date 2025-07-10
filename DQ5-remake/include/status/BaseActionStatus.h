#pragma once

namespace status {
    struct BaseActionStatus {
        static int work_;
        static int flag_;  
        static void clear();
    };
}
