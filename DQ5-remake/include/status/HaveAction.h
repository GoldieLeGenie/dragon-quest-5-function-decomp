#pragma once

#include <cstdint>
#include "status/BaseHaveAction.h"

namespace status {

    struct HaveAction {
        enum ActionMode : int32_t
        {
            TownMode = 0x0,
            BattleMode = 0x1,
        };
        BaseHaveAction action_[18];  // 0x00–0x6B
        uint16_t index_;             // 0x6C
        uint16_t kind_;              // 0x6E
        uint16_t level_;             // 0x70

        static int mode_;
        static int addRulaLevel_;
        static int addParupunteLevel_;

        static bool isBattleMode();
        static void setBattleMode();
        static void sort(HaveAction* self, int index);
        static void clear(HaveAction* self);
        static void setup(HaveAction* self, int index, int level);
        ~HaveAction();
        HaveAction();
        static void add(HaveAction* self, int battleAction, int usuallyAction, bool flag);
        static void clearRememberingAction(HaveAction* self);
        static int getRememberingAction(HaveAction* self);
        static int getHaveAction(HaveAction* self, int index);
        static int getAction(HaveAction* self, int index);
        static bool isHaveAction(HaveAction* self, int actionIndex);
        static bool isAction(HaveAction* self, int actionIndex);
        static int getCount(HaveAction* self);
        static int getActionAll(HaveAction* self, int index);
        static int getCountAll(HaveAction* self);
        static int getBattleAction(HaveAction* self, int index);
        static int getTownAction(HaveAction* self, int index);
        static void del(HaveAction* self, int index);
        static void levelup(HaveAction* self, int index, int level, bool flag);
        static void execLevelup(HaveAction* self, int level, bool flag);
        static void loadLevelup(HaveAction* self, int level, int rulaLevel, int parupunteLevel);
        static void debugLevelup(HaveAction* self, int level);
        static void cleanup(HaveAction* self, int index);
        static void setActionMode(status::HaveAction::ActionMode md);
        static void setTownMode();
        static bool isTownMode();
        static int getActionMode();
    };

} 