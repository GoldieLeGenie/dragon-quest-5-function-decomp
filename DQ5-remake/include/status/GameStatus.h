#pragma once
#include "iostream"
#include "dq5/Language.h"

namespace status {
	struct GameStatus {
		dq5::Language language_;
		uint32_t playTime_;
		uint32_t uniqueID_;
		void addPlayTime(GameStatus* self, uint32_t time);
		void initialize(GameStatus* self);
		uint32_t getPlayTime(GameStatus* self);
		void setPlayTime(GameStatus* self, uint32_t time);
		void resetUniqueID(GameStatus* self);
		uint32_t getUniqueID(GameStatus* self);
		void setUniqueID(GameStatus* self, uint32_t id);
		GameStatus();
		~GameStatus();
	};
}