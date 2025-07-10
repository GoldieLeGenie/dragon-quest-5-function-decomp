#pragma once
#include <cstdint>
#include "ar/File.h"
#include "args/DataObject.h"

namespace args {

    struct ExcelBinaryData {
        uint32_t id_;
        uint32_t record_;
        uint32_t size_;
        uint32_t cash_;
        uint32_t current_;

        void setupHeader(const char* fname, ar::File::LoadSwitch loadSwitch);
        void* setup();                   
        void setup(uint32_t allocsize);
        void readRecord(void* addr, const char* fname, uint32_t seek, uint32_t cashsize, ar::File::LoadSwitch loadSwitch);
        void* readFileData(DataObject* dat, char* fname);
        void* getRecord(int index, void* addr, const char* fname, ar::File::LoadSwitch loadSwitch);
        void freeRecord(void* addr);
        void clearRecord(void* addr);
        void clearData(DataObject* dat);
        void cleanup(void* addr);
        static void* checkSum(void* addr, int id);
        void* allocRecord(uint32_t size);

    };

}
