#include "status/ShopList.h"
#include "dq5/ShopData1.h"
#include "dq5/ShopData2.h"
#include "dq5/ShopData3.h"
#include "dq5/ShopDataSugoroku.h"


int status::ShopList::section_ = 0;
int status::ShopDataIndex = 0;

void status::ShopList::initialize(ShopList* self)
{
	status::ShopList::section_ = 0;
}

int status::ShopList::getDataIndex(const ShopList* self, int shop)
{
    int rowBase = ShopDataIndex << 6;

    switch (shop)
    {
    case 1:  return rowBase | 0x01; 
    case 2:  return rowBase | 0x02;
    case 3:  return rowBase | 0x08;
    case 4:  return rowBase | 0x0E;
    case 5:  return rowBase | 0x14;
    case 6:  return rowBase | 0x1A;
    case 7:  return rowBase | 0x20;
    case 8:  return rowBase | 0x21;
    case 9:  return rowBase | 0x22;
    case 10: return rowBase | 0x28;
    case 11: return rowBase | 0x34;
    case 12: return rowBase | 0x2E;
    case 13: return rowBase | 0x3A;
    default: return rowBase; 
    }
}

int status::ShopList::getShopCount(const status::ShopList* self, int shop)
{
    int dataIndex = status::ShopList::getDataIndex(self, shop);
    int count = 0;

    auto loadSwitchCast = [](int value) {
        return static_cast<ar::File::LoadSwitch>(value);
        };

    for (int i = 0; i < 6; ++i)
    {
        std::uint16_t* record = nullptr;

        switch (status::ShopList::section_)
        {
        case 0:
            record = reinterpret_cast<std::uint16_t*>(
                dq5::level::ShopData1::binary_.getRecord(
                    dataIndex + i,
                    dq5::level::ShopData1::addr_,
                    dq5::level::ShopData1::filename_[0],
                    loadSwitchCast(dq5::level::ShopData1::loadSwitch_)));
            break;

        case 1:
            record = reinterpret_cast<std::uint16_t*>(
                dq5::level::ShopData2::binary_.getRecord(
                    dataIndex + i,
                    dq5::level::ShopData2::addr_,
                    dq5::level::ShopData2::filename_[0],
                    loadSwitchCast(dq5::level::ShopData2::loadSwitch_)));
            break;

        case 2:
            record = reinterpret_cast<std::uint16_t*>(
                dq5::level::ShopData3::binary_.getRecord(
                    dataIndex + i,
                    dq5::level::ShopData3::addr_,
                    dq5::level::ShopData3::filename_[0],
                    loadSwitchCast(dq5::level::ShopData3::loadSwitch_)));
            break;

        default:
            record = reinterpret_cast<std::uint16_t*>(
                dq5::level::ShopDataSugoroku::binary_.getRecord(
                    dataIndex + i,
                    dq5::level::ShopDataSugoroku::addr_,
                    dq5::level::ShopDataSugoroku::filename_[0],
                    loadSwitchCast(dq5::level::ShopDataSugoroku::loadSwitch_)));
            break;
        }

        if (record && record[2] != 0)
            ++count;
    }

    return count;
}



int status::ShopList::getShopItem(const ShopList* self, int shop, int index)
{
    int dataIndex = status::ShopList::getDataIndex(self, shop);
    void* addr = nullptr;
    const char* filename = nullptr;
    ar::File::LoadSwitch loadSwitch;
    args::ExcelBinaryData* binary = nullptr;

    switch (status::ShopList::section_)
    {
    case 0:
        binary = &dq5::level::ShopData1::binary_;
        addr = dq5::level::ShopData1::addr_;
        filename = dq5::level::ShopData1::filename_[0];
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData1::loadSwitch_);
        break;
    case 1:
        binary = &dq5::level::ShopData2::binary_;
        addr = dq5::level::ShopData2::addr_;
        filename = dq5::level::ShopData2::filename_[0];
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData2::loadSwitch_);
        break;
    case 2:
        binary = &dq5::level::ShopData3::binary_;
        addr = dq5::level::ShopData3::addr_;
        filename = dq5::level::ShopData3::filename_[0];
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData3::loadSwitch_);
        break;
    default:
        binary = &dq5::level::ShopDataSugoroku::binary_;
        addr = dq5::level::ShopDataSugoroku::addr_;
        filename = dq5::level::ShopDataSugoroku::filename_[0];
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopDataSugoroku::loadSwitch_);
        break;
    }

    void* record = binary->getRecord(dataIndex + index, addr, filename, loadSwitch);

    return static_cast<int>(reinterpret_cast<std::uint16_t*>(record)[2]);
}


status::ShopList::ChurchType status::ShopList::getChurchType(const status::ShopList* self, bool second)
{
   
    int offset = second ? 33 : 1;
    int recordIndex = offset | (ShopDataIndex << 6);

    void* addr = nullptr;
    const char* filename = nullptr;
    ar::File::LoadSwitch loadSwitch;
    args::ExcelBinaryData* binary = nullptr;

    switch (status::ShopList::section_) {
    case 0:
        binary = &dq5::level::ShopData1::binary_;
        filename = dq5::level::ShopData1::filename_[0];
        addr = dq5::level::ShopData1::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData1::loadSwitch_);
        break;
    case 1:
        binary = &dq5::level::ShopData2::binary_;
        filename = dq5::level::ShopData2::filename_[0];
        addr = dq5::level::ShopData2::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData2::loadSwitch_);
        break;
    case 2:
        binary = &dq5::level::ShopData3::binary_;
        filename = dq5::level::ShopData3::filename_[0];
        addr = dq5::level::ShopData3::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData3::loadSwitch_);
        break;
    default:
        binary = &dq5::level::ShopDataSugoroku::binary_;
        filename = dq5::level::ShopDataSugoroku::filename_[0];
        addr = dq5::level::ShopDataSugoroku::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopDataSugoroku::loadSwitch_);
        break;
    }

    void* record = binary->getRecord(recordIndex, addr, filename, loadSwitch);
    int32_t rawValue = *reinterpret_cast<int32_t*>(record);

    switch (static_cast<ChurchType>(rawValue)) {
    case ChurchType::NoExist:
    case ChurchType::Exist:
    case ChurchType::NoPray:
        return static_cast<ChurchType>(rawValue);
    default:
        return ChurchType::Exist;  
    }
}


int status::ShopList::getHotelPrice(const ShopList* self, bool second)
{
    int recordIndex = (ShopDataIndex << 6) | (32 * static_cast<int>(second));

    void* addr = nullptr;
    const char* filename = nullptr;
    ar::File::LoadSwitch loadSwitch;
    args::ExcelBinaryData* binary = nullptr;

    switch (status::ShopList::section_) {
    case 0:
        binary = &dq5::level::ShopData1::binary_;
        filename = dq5::level::ShopData1::filename_[0];
        addr = dq5::level::ShopData1::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData1::loadSwitch_);
        break;
    case 1:
        binary = &dq5::level::ShopData2::binary_;
        filename = dq5::level::ShopData2::filename_[0];
        addr = dq5::level::ShopData2::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData2::loadSwitch_);
        break;
    case 2:
        binary = &dq5::level::ShopData3::binary_;
        filename = dq5::level::ShopData3::filename_[0];
        addr = dq5::level::ShopData3::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopData3::loadSwitch_);
        break;
    default:
        binary = &dq5::level::ShopDataSugoroku::binary_;
        filename = dq5::level::ShopDataSugoroku::filename_[0];
        addr = dq5::level::ShopDataSugoroku::addr_;
        loadSwitch = static_cast<ar::File::LoadSwitch>(dq5::level::ShopDataSugoroku::loadSwitch_);
        break;
    }

    void* record = binary->getRecord(recordIndex, addr, filename, loadSwitch);
    return *reinterpret_cast<int32_t*>(record);
}


status::ShopList::ShopList()
{
    
}

status::ShopList::~ShopList()
{
    
}

int status::ShopList::getAreaOffset(int area)
{
    return area << 6;
}