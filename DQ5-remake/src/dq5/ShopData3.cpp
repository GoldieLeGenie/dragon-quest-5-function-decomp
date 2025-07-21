#include "dq5/ShopData3.h"

namespace dq5::level {

	args::ExcelBinaryData ShopData3::binary_;
	void* ShopData3::addr_ = nullptr;
	const char* ShopData3::filename_[] = { "./LEVELDATA/dq5ds_shop_data_3.dat" };
	int ShopData3::loadSwitch_ = 1;
	ShopData3* ShopData3_ = nullptr;



}