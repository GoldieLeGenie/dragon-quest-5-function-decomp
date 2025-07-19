#include "dq5/SplitMessage.h"

namespace dq5::level {
	args::ExcelBinaryData SplitMessage::binary_;
	void* SplitMessage::addr_ = nullptr;
	const char* SplitMessage::filename_[] = { "./LEVELDATA/dq5ds_split_message.dat" };
	int SplitMessage::loadSwitch_ = 1;
	SplitMessage* SplitMessage = nullptr;
	}