#pragma once

namespace status {

    struct UseActionFlag {
        static bool doubleFlag_;  // déclaration seulement (pas de définition ici)
        static int flag_;  // <- déclaration du flag
        static void setZaoraruInvalid(bool flag);
        static bool eventBattle_;
        static bool multiFlag_;
        static bool timeReverseFlag_;
    };

}
