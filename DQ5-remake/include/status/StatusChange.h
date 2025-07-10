#pragma once

#include "status/StatusChangeOne.h"
#include "dq5/ActionParam.h"
#include "status/StatusChangeOne.h"

namespace status {

    struct StatusChange {
        StatusChangeOne status_[51];
        static bool isEnable(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel);
        static StatusChangeOne statusFizzleZone_;
        static StatusChangeOne statusToherosu_;
        static StatusChangeOne statusSinobiasi_;
        static StatusChangeOne statusMonstersFood_;
        static StatusChangeOne statusCloseDoor_;
        static StatusChangeOne statusTimeStop_;
    };


} 