#include "args/ExcelBinaryData.h"
#include "args/FileUtils.h"  
#include <cstdint>
#include <cstdlib>
#include "args/DataObject.h"
#include <cstring>  

void args::ExcelBinaryData::setupHeader(const char* fname, ar::File::LoadSwitch) {
        uint32_t addr[5] = {};  

        g_File_load(fname, addr, 0, sizeof(addr));

        this->id_ = addr[0];
        this->record_ = addr[1];
        this->size_ = addr[2];
        this->cash_ = addr[3];
        this->current_ = addr[4];
}

void* args::ExcelBinaryData::setup() {
    void* result = std::malloc(static_cast<size_t>(size_) * cash_);
    current_ = static_cast<uint32_t>(-1); 
    return result;
}


void args::ExcelBinaryData::setup(uint32_t allocsize) {
    current_ = static_cast<uint32_t>(-1);  

    uint32_t total = size_ * cash_;
    if (total > allocsize && size_ != 0) {
        cash_ = allocsize / size_;
    }
}

void args::ExcelBinaryData::readRecord(
    void* addr,
    const char* fname,
    uint32_t seek,
    uint32_t cashsize,
    ar::File::LoadSwitch )
{
    if (seek + cashsize >= record_) {
        seek = record_ > cashsize ? record_ - cashsize : 0;
    }

    current_ = seek;

    uint32_t offset = size_ * seek + 20;
    uint32_t length = size_ * cashsize;

    g_File_load(fname, addr, offset, length);
}

void* args::ExcelBinaryData::readFileData(DataObject* dat, char* fname) {
    void* result = MG_LoadFile(fname, 0);
    dat->m_addr = result;
    dat->m_flag = 1;
    return result;
}


void* args::ExcelBinaryData::getRecord(int index, void* addr, const char* fname, ar::File::LoadSwitch /*loadSwitch*/) {
        if (index < 0 || addr == nullptr || fname == nullptr || size_ == 0)
            return nullptr;

        uint32_t v11, v12;

        if (current_ > static_cast<uint32_t>(index) || current_ == static_cast<uint32_t>(-1) || current_ + cash_ <= static_cast<uint32_t>(index)) {
            std::memset(addr, 0, cash_ * size_);

            int readStart = index;
            if (cash_ + index >= record_)
                readStart = record_ > cash_ ? record_ - cash_ : 0;

            current_ = readStart;

            uint32_t offset = size_ * readStart + 20;
            uint32_t length = size_ * cash_;

            g_File_load(fname, addr, offset, length);

            v12 = size_;
            v11 = index - current_;
        }
        else {
            v11 = size_;
            v12 = index - current_;
        }

        return static_cast<char*>(addr) + v11 * v12;
    }

void args::ExcelBinaryData::freeRecord(void* addr) {
    FreeFileMemory_0(addr);
}

void args::ExcelBinaryData::clearRecord(void* addr) {
    if (addr != nullptr && size_ > 0 && cash_ > 0) {
        std::memset(addr, 0, cash_ * size_);
    }
}


void args::ExcelBinaryData::clearData(DataObject* dat) {
    if (dat && dat->m_addr && dat->m_flag) {
        FreeFileMemory_0(dat->m_addr);
        dat->m_addr = nullptr;
        dat->m_flag = 0;
    }
}

void args::ExcelBinaryData::cleanup(void* addr) {
    FreeFileMemory_0(addr);
}

void* args::ExcelBinaryData::checkSum(void* addr, int id) {
    return static_cast<char*>(addr) + 4;
}

void* args::ExcelBinaryData::allocRecord(uint32_t size) {
    return std::malloc(size_ * size);
}

