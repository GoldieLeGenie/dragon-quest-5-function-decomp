#include "status/BaseHaveAction.h"



status::BaseHaveAction::~BaseHaveAction()
{
}

bool status::BaseHaveAction::isEnable(BaseHaveAction* self)
{
	return self->battleAction_ || self->usuallyAction_ != 0;
}

void status::BaseHaveAction::add(BaseHaveAction* self, int battleAction, int usuallyAction)
{
	uint8_t actionFlag; 

	self->usuallyAction_ = usuallyAction;
	actionFlag = self->actionFlag_;
	self->battleAction_ = battleAction;
	self->actionFlag_ = actionFlag | 4;
}


void status::BaseHaveAction::remembering(BaseHaveAction* self) {
	if (self->battleAction_ != 0 || self->usuallyAction_ != 0) {
		self->actionFlag_ |= 0x08;  // active le bit "remember"
	}
}

bool status::BaseHaveAction::isRemembering(BaseHaveAction* self)
{
	bool isEmptyBattleAction;

	isEmptyBattleAction = self->battleAction_ == 0;
	if (!self->battleAction_)
		isEmptyBattleAction = self->usuallyAction_ == 0;
	return !isEmptyBattleAction && (self->actionFlag_ & 8) != 0;
}

void status::BaseHaveAction::clearRemembering(BaseHaveAction* self)
{
	if ((self->actionFlag_ & 8) != 0)
		self->actionFlag_ &= ~0xF7;
}

void status::BaseHaveAction::del(BaseHaveAction* self)
{
	self->actionFlag_ = 0;
	self->battleAction_ = 0;
}


