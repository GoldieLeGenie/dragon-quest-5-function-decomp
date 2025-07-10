#include "status/HaveAction.h"

int status::HaveAction::mode_ = 0;

bool status::HaveAction::isBattleMode() {
        return mode_ == 1;
    }

void status::HaveAction::setMode(int value) {
        mode_ = value;
    }

