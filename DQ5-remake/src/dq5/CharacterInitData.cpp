#include "dq5/CharacterInitData.h"

namespace dq5::level {

	args::ExcelBinaryData CharacterInitData::binary_;
	void* CharacterInitData::addr_ = nullptr;
	const char* CharacterInitData::filename_[] = { "./LEVELDATA/dq5ds_character_init.dat" };
	int CharacterInitData::loadSwitch_ = 1;

}