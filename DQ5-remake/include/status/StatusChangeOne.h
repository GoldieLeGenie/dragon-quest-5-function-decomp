#pragma once
#include <cstdint>
#include "ar/BitFlag.h" // Assurez-vous que la classe ar::Flag est bien incluse
#include "dq5/ActionParam.h"
namespace status {

    struct StatusChangeOne {
        ar::Flag32 flag_;         // 0x00
        int16_t actionIndex_;   // 0x04
        uint8_t damageType_;    // 0x06
        uint8_t count_;         // 0x07
        uint8_t turn_;          // 0x08
        int8_t pattern_;        // 0x09
        int8_t astron_;         // 0x0A
        uint8_t padding_ = 0;   // 0x0B (remplissage pour alignement si nécessaire)
        ~StatusChangeOne();
        static bool isEnable(const StatusChangeOne* self);
        void StatusChangeOneInit(StatusChangeOne* self);
        static void clear(StatusChangeOne* self);
        static int addTurn(StatusChangeOne* self);
        static void setup(StatusChangeOne* self, dq5::level::ActionParam::ACTIONTYPE release, int actionIndex, bool flag);
        static bool isValidOnAstron(int damageType);
        static bool isValidOnSpazz(int damageType);
        static bool isAfterOnSleep(int damageType);
        static bool isAfterOnPath1(int damageType);
        static bool isAfterOnConfuze(int damageType);
        static bool isAfterOnPowerSave(int damageType);
        static void cleanup(StatusChangeOne* self);
        static bool isCancel(StatusChangeOne* self);
        static void execStartOfTurn(StatusChangeOne* self);
        static void setEnable(StatusChangeOne* self, bool flag);
        static void setRelease(StatusChangeOne* self, bool flag);
        static void execEndOfTurn(StatusChangeOne* self);
        static void execEndOfRound(StatusChangeOne* self);
        static void execWalk(StatusChangeOne* self);
        static bool isRelease(const StatusChangeOne* self);
        static int getExecMessage(StatusChangeOne* self);
        static int getExecMessage2(StatusChangeOne* self);
        static int getEffectMessage(int index, int actionIndex);
        static int getResultPlayerMessage(StatusChangeOne* self);
        static int getResultMonsterMessage(StatusChangeOne* self);
        static int getAgainEnableMessage(StatusChangeOne* self);
        static int getAgainDisableMessage(StatusChangeOne* self);
        static int getReleaseMessage(StatusChangeOne* self);
        

    };
 
    extern int actionType_;
    extern uintptr_t effectMessage_;
    
} // namespace status
