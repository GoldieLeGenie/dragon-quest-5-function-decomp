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



