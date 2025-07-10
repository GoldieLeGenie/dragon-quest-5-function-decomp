#include "status/HaveStatusInfo.h"
#include "status/HaveStatusInfo_0.h"
#include "status/StatusChange.h"
#include "status/StatusChangeOne.h"
#include "dq5/ActionParam.h"
#include "ar/BitFlag.h"



status::StatusChangeOne status::StatusChange::statusFizzleZone_;
status::StatusChangeOne status::StatusChange::statusCloseDoor_;
status::StatusChangeOne status::StatusChange::statusToherosu_;
status::StatusChangeOne status::StatusChange::statusSinobiasi_;
status::StatusChangeOne status::StatusChange::statusMonstersFood_;
status::StatusChangeOne status::StatusChange::statusTimeStop_;





bool status::StatusChange::isEnable(StatusChange* self, dq5::level::ActionParam::ACTIONTYPE rel)
{
    const StatusChangeOne* ptr = &StatusChange::statusFizzleZone_;

    switch (rel) {
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_FIZZLEZONE:
        return StatusChangeOne::isEnable(ptr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TOHEROSU:
        ptr = &StatusChange::statusToherosu_;
        return StatusChangeOne::isEnable(ptr);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_SINOBIASI:
        return StatusChangeOne::isEnable(&StatusChange::statusSinobiasi_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_NORMAL_MAMONONOESA:
        return StatusChangeOne::isEnable(&StatusChange::statusMonstersFood_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CLOSEDOOR:
        return StatusChangeOne::isEnable(&StatusChange::statusCloseDoor_);

    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_TIMESTOP:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE4:
    case dq5::level::ActionParam::ACTIONTYPE::ACTIONTYPE_CURSE5:
        ptr = &StatusChange::statusTimeStop_;
        return StatusChangeOne::isEnable(ptr);

    default:
        ptr = &self->status_[static_cast<int>(rel)];
        return StatusChangeOne::isEnable(ptr);
    }
}
