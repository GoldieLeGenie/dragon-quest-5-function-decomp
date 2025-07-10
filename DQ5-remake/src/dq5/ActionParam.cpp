#include "dq5/ActionParam.h"

namespace dq5::level {

	args::ExcelBinaryData ActionParam::binary_;
	void* ActionParam::addr_ = nullptr;
	const char* ActionParam::filename_[] = { "./LEVELDATA/dq5ds_action.dat" };
	int ActionParam::loadSwitch_ = 1;

}