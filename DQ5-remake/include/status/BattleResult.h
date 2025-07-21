#pragma once 
#include "dq5/CharacterType.h"
#include "ar/FlagArray.h"

namespace status {
	struct BattleResult {
		struct Result
		{
			uint16_t monsterCount_;  
			uint16_t itemCount_;     
			uint8_t level_;          
		};
		status::BattleResult::Result result_[202];
		ar::FlagArray<202> encountFlag_;
		bool playerVictory_;
		bool playerDemolition_;
		int playerDemolitionMessage_;
		bool disablePlayerDemolition_;
		int battleTurnCount_;
		bool battleExecute_;
		bool demolitionPartyTalk_;
		dq5::level::CharacterType firstAttack_;
		dq5::level::CharacterType debugFirstAttack_;
		static int bookDataEntryCount;
		static int getMonsterBookIndex(BattleResult* self, int monsterIndex);
		static int getMonsterFriendly(BattleResult* self, int index);
		static bool isDisablePlayerDemolition(BattleResult* self);
		static void setDisablePlayerDemolition(BattleResult* self, bool flag);
		static int16_t getMonsterItemIndex(BattleResult* self, int index);
		static uint32_t getMonsterTotalGold(BattleResult* self, int index);
		static uint32_t getMonsterGold(BattleResult* self, int index);
		static uint32_t getMonsterTotalExp(BattleResult* self, int index);
		static uint32_t getMonsterExp(BattleResult* self, int index);
		static int getMonsterEncountCount(BattleResult* self);
		static bool isMonsterEncount(const BattleResult* self, uint32_t index);
		static void regenesisMonsterLevel(BattleResult* self, int index, uint8_t level);
		static int getMonsterIndex(BattleResult* self, int index);
		static void addMonsterItemCount(BattleResult* self, int index);
		static void addMonsterCount(BattleResult* self, int index);
		static void setMonsterEncount(BattleResult* self, uint32_t index, bool flag);
		static void initialize(BattleResult* self);
	};
}