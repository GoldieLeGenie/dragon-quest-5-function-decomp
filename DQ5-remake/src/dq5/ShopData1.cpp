#include "dq5/ShopData1.h"

namespace dq5::level {

	args::ExcelBinaryData ShopData1::binary_;
	void* ShopData1::addr_ = nullptr;
	const char* ShopData1::filename_[] = { "./LEVELDATA/dq5ds_shop_data_1.dat" };
	int ShopData1::loadSwitch_ = 1;
	ShopData1* ShopData1_ = nullptr;



}
