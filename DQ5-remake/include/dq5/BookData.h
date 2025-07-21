#pragma once 
#include "iostream"
#include "args/ExcelBinaryData.h"


namespace dq5::level {
	struct BookData
	{
		static args::ExcelBinaryData binary_;
		static void* addr_;
		static const char* filename_[];
		static int loadSwitch_;

		uint16_t index;     
		uint8_t friendly;  
		uint8_t dummy0;
	};
}