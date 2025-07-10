#pragma once
#include "args/DataObject.h"
#include "args/PlacementParameter.h"
#include "args/ScriptParam.h"

namespace args {
	struct ScriptObject {
		DataObject dataObject_;
		PlacementParameter placeParam_;
		ScriptParam initializeScriptParam_;
		ScriptParam executeScriptParam_;
		ScriptParam terminateScriptParam_;
		int ctrlId_;    // true si data chargée
	};
}

