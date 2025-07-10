#include "status/UseActionMessage.h"
#include <cstring> // pour memset

void status::UseActionMessage::clear(UseActionMessage* self) {
    std::memset(self, 0, sizeof(status::UseActionMessage));
}
