#pragma once
#include "iostream"
#include "dq5/Language.h"

namespace status {
	struct GameStatus {
		dq5::Language language_;
		uint32_t playTime_;
		uint32_t uniqueID_;
		static void addPlayTime(GameStatus* self, uint32_t time);
		static void initialize(GameStatus* self);
		static uint32_t getPlayTime(GameStatus* self);
		static void setPlayTime(GameStatus* self, uint32_t time);
		static void resetUniqueID(GameStatus* self);
		static uint32_t getUniqueID(GameStatus* self);
		static void setUniqueID(GameStatus* self, uint32_t id);
		GameStatus();
		~GameStatus();
	};
}