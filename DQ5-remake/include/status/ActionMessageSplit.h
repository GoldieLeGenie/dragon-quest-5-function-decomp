#pragma once
#include "status/CharacterStatus.h"
#include "status/UseActionMessage.h"

namespace status {
	struct ActionMessageSplit {
		static int getMessageSpecial(int splitIndex);
		static char splitFlag_;
		static int getMessageZero(int splitIndex);
		static int getMessageRandom(int splitIndex);
		static int actionIndex_;
		static int getMessageNoTarget(int splitIndex);
		static int getMessagePlayerOne(status::CharacterStatus* target, int splitIndex);
	};

	static int splitMessage_;
}