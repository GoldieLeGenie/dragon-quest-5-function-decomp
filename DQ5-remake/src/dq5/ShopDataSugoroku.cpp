#include "dq5/ShopDataSugoroku.h"

namespace dq5::level {

    args::ExcelBinaryData ShopDataSugoroku::binary_;
    void* ShopDataSugoroku::addr_ = nullptr;
    const char* ShopDataSugoroku::filename_[] = { "./LEVELDATA/dq5ds_shop_data_sugoroku.dat" };
    int ShopDataSugoroku::loadSwitch_ = 1;
    ShopDataSugoroku* ShopDataSugoroku_ = nullptr;
}