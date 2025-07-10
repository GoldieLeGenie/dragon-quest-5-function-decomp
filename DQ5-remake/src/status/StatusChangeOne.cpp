#include "status/StatusChangeOne.h"
#include "ar/BitFlag.h"


bool status::StatusChangeOne::isEnable(const StatusChangeOne* self) {
    return self->flag_.test(0);
}
