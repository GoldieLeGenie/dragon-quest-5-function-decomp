#include "dq5/EffectMessage.h"

namespace dq5::level {

	args::ExcelBinaryData EffectMessage::binary_;
	void* EffectMessage::addr_ = nullptr;
	const char* EffectMessage::filename_[] = { "./LEVELDATA/dq5ds_effect_message.dat" };
	int EffectMessage::loadSwitch_ = 1;

}