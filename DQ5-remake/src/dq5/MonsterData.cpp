#include "dq5/MonsterData.h"

namespace dq5::level {

	args::ExcelBinaryData MonsterData::binary_;
	void* MonsterData::addr_ = nullptr;
	const char* MonsterData::filename_[] = { "./LEVELDATA/dq5ds_monster.dat" };
	int MonsterData::loadSwitch_ = 1;
	MonsterData* monsterData2_ = nullptr;

    

}
