#pragma once
#include "status/CharacterStatus.h"
#include "status/UseActionParam.h"

namespace status {

    // Déclaration anticipée du type utilisé dans la signature
    struct UseActionParam;

    struct BaseAction {
        static int useActionParam_;
        static int actionIndex_;
        static int executeAction_;
        static int resultFlag_;
        static int executeActor_;
        static int executeTarget_;

        /*static void exec(UseActionParam* useActionParam, bool isBattleMode);
        static bool execMainRoot(status::CharacterStatus* actor, status::CharacterStatus* target);
        static void execActionSetup();
        static int execActionExecute();*/
    };

} // namespace status
