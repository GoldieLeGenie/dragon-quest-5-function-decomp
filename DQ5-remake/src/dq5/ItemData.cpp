#include "dq5/ItemData.h"

namespace dq5::level {

	args::ExcelBinaryData ItemData::binary_;
	void* ItemData::addr_ = nullptr;
	const char* ItemData::filename_[] = { "./LEVELDATA/dq5ds_item_list.dat" };
	int ItemData::loadSwitch_ = 1;

} 
