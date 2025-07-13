#pragma once
#include <cstdint>

namespace status {

    struct UseActionFlag {
        static bool doubleFlag_;  // déclaration seulement (pas de définition ici)
        static int flag_;  // <- déclaration du flag
        static void setZaoraruInvalid(bool flag);
        static bool eventBattle_;
        static bool multiFlag_;
        static bool timeReverseFlag_;
        static int getExecActionCount(int actionIndex);
        static void addExecActionCount(int actionIndex);
        static uint16_t execActionCount_[502];
        static int callFriendMonsterIndex_;
        static int workParam_;
        static int getCallFriendMonsterIndex();
        static int getCallFriendMonsterGroup();
        static bool isCallFriend();
        static void setCallFriend(bool flag, int monsterGroup, int monsterIndex);
        static bool isZaoraruInvalid();
        static void clear();
        static void clearExecActionCount();
        static void setBreakPrayRing(bool flag);
        static int isBreakPrayRing();
        static void setFailedNoUseMp(bool flag);
        static int isFailedNoUseMp();
        UseActionFlag();
        ~UseActionFlag() = default;
    };

}
