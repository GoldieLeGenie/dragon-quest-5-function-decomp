#include "dq5/BookData.h"


namespace dq5::level {

	args::ExcelBinaryData BookData::binary_;
	void* BookData::addr_ = nullptr;
	const char* BookData::filename_[] = { "./LEVELDATA/dq5ds_book_data.dat" };
	int BookData::loadSwitch_ = 1;
	BookData* BookData_ = nullptr;



}
