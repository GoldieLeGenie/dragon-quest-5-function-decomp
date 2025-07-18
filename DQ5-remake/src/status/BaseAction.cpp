#include "status/BaseAction.h"
#include "status/UseActionFlag.h"
#include "status/UseActionParam.h"
#include "status/UseActionMessage.h"
#include "status/CharacterStatus.h"
#include "status/ActionCheckActor.h"
#include "status/ActionCheckTarget.h"
#include "status/ActionExecAfter.h"
#include "status/ActionExecBefore.h"
#include "status/ActionMessage.h"
#include "status/ActionMessageSplit.h"
#include "status/StatusChange.h"
#include "status/BaseActionStatus.h"
#include "dq5/ActionParam.h"
#include "status/UseActionFlag.h"
#include "status/HaveStatusInfo_0.h"
#include "status/HaveStatusInfo.h"
#include "status/ActionEffectValue.h"

int status::BaseAction::useActionParam_ = 0;
int status::BaseAction::actionIndex_ = 0;
int status::BaseAction::executeAction_ = 0;
int status::BaseAction::resultFlag_ = 0;




void status::BaseHaveAction::clear(BaseHaveAction* self)
{
	self->actionLevel_ = 0;
	self->battleAction_ = 0;
}


int status::BaseHaveAction::getAction(BaseHaveAction* self)
{
	return self->battleAction_;
}

int status::BaseHaveAction::getUsuallyAction(BaseHaveAction* self)
{
	return self->usuallyAction_;
}

bool status::BaseHaveAction::isRemember(BaseHaveAction* self)
{
	bool noBattleAction = (self->battleAction_ == 0);
	if (noBattleAction)
		noBattleAction = (self->usuallyAction_ == 0);

	return !noBattleAction && ((self->actionFlag_ & 4) != 0);
}


