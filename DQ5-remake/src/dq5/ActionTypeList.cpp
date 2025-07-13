#include "dq5/ActionTypeList.h"

namespace dq5::level {

	args::ExcelBinaryData ActionTypeList::binary_;
	void* ActionTypeList::addr_ = nullptr;
	const char* ActionTypeList::filename_[] = { "./LEVELDATA/dq5ds_action_type.dat" };
	int ActionTypeList::loadSwitch_ = 1;

}