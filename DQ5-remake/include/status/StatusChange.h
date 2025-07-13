#pragma once

#include "status/StatusChangeOne.h"
#include "dq5/ActionParam.h"
#include "dq5/CharacterType.h"
#include "status/StatusChangeOne.h"

namespace status {

    struct StatusChange {
        StatusChangeOne status_[51];
        static bool isEnable(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        ~StatusChange();
        static StatusChangeOne statusFizzleZone_;
        static StatusChangeOne statusToherosu_;
        static StatusChangeOne statusSinobiasi_;
        static StatusChangeOne statusMonstersFood_;
        static StatusChangeOne statusCloseDoor_;
        static StatusChangeOne statusTimeStop_;
        static void setup(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE re, bool flag);
        static bool isValidBefore(StatusChange* self, int damageType);
        static void checkValidAfter(StatusChange* self, int damageType);
        static void release(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static bool isRelease(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static int getActionIndex(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static int getReleaseMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static void cleanup(status::StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static void StatusChangeInit(StatusChange* self);
        static void restore(status::StatusChange* self, int index);
        static void clear(StatusChange* self);
        static void store(StatusChange* self, int index);
        static bool setup(StatusChange* self, int actionIndex, bool flag);
        static void execWalk(status::StatusChange* self);
        static void execStartOfTurn(StatusChange* self);
        static void execEndOfTurn(status::StatusChange* self);
        static void execEndOfRound(status::StatusChange* self);
        static int getExecMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static int getExecMessage2(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static int getResultMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType, dq5::level::CharacterType characterType);
        static int getResultMessage2(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType, dq5::level::CharacterType characterType);
        static int getAgainDisableMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType);
        static int getAgainEnableMessage(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType);
        static void setTurn(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType, uint8_t turn);
        static void printAll(StatusChange* self);
        static void setCount(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE status, uint8_t count);
        static void execEndOfRoundForAll(StatusChange* self);
        static void execEndOfBattle(StatusChange* self);
        static void StaticClear();
        static int getRestTurn(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE actionType);
        static uint8_t ToherosCount_;
        static uint16_t ToherosTurn_;

        static uint8_t SinobiasCount_;
        static uint16_t SinobiasTurn_;

        static uint8_t MonstersFoodCount_;
        static uint16_t MonstersFoodTurn_;

        static int getToherosInfo();
        static int getSinobiasiInfo();
        static int getMonstersFoodInfo();
        static void setToherosInfo(uint16_t value);
        static StatusChangeOne* getStatusChangeOne(StatusChange* self, int rel);
        static StatusChangeOne* getStatusChangeOneForOnlyOne(StatusChange* self, int rel);
        static void cleanupCloseDoor();
        static void cleanupFizzleZone();
        static void copy(StatusChange* self, StatusChange* status);
        static void setupCloseDoor();
        static void setupFizzleZone();
        static void setMonstersFoodInfo(uint16_t value);
        static void print(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel, uint8_t* name);
        dq5::level::ActionParam::ACTIONTYPE isRelease(StatusChange* current);
        static bool isEnableFizzleZone();
        static bool isEnableCloseDoor();
        static bool isCancel(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static int getTurn(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
    };
    extern uint8_t TIMESTOP;

} 