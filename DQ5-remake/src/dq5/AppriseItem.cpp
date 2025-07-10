#include "dq5/AppriseItem.h"

namespace dq5::level {

	args::ExcelBinaryData AppriseItem::binary_;
	void* AppriseItem::addr_ = nullptr;
	const char* AppriseItem::filename_[] = { "./LEVELDATA/dq5ds_apprise_item.dat" };
	int AppriseItem::loadSwitch_ = 1;

}