#include "dq5/ShopData2.h"

namespace dq5::level {

	args::ExcelBinaryData ShopData2::binary_;
	void* ShopData2::addr_ = nullptr;
	const char* ShopData2::filename_[] = { "./LEVELDATA/dq5ds_shop_data_2.dat" };
	int ShopData2::loadSwitch_ = 1;
	ShopData2* ShopData2_ = nullptr;



}
