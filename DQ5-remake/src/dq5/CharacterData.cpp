#include "dq5/CharacterData.h"

namespace dq5::level {

	args::ExcelBinaryData CharacterData::binary_;
	void* CharacterData::addr_ = nullptr;
	const char* CharacterData::filename_[] = { "./LEVELDATA/dq5ds_player_level%d.dat" }; 
	int CharacterData::loadSwitch_ = 0;

}
