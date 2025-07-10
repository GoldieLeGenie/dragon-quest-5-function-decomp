#include "status/BaseActionStatus.h"


int status::BaseActionStatus::work_ = 0;
int status::BaseActionStatus::flag_ = 0;

void status::BaseActionStatus::clear() {
    status::BaseActionStatus::work_ = 0;
}
