
#include "status/UseActionParam.h"

namespace status {
    struct ActionCheckActor {
        static void checkActorImmediateItem(UseActionParam* param);
        static bool isAlive(UseActionParam* useActionParam);
    };
}