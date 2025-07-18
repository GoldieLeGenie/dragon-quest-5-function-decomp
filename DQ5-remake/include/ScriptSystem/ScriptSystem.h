#pragma once
#include "args/DataObject.h"
#include "args/ScriptEngine.h"

struct ScriptSystem {
	bool flag_;
	int chapter_;
	args::DataObject dataObject_;       
	args::ScriptEngine scriptEngine_;   
	bool executeEnable_;
	bool enable_;        
	static void initialize(ScriptSystem* self,int chapter, const char* filename);
};