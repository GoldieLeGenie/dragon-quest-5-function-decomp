#pragma once 

namespace status {
	struct OptionStatus {
		char sackSort_;
		char bgmVolume_;
		char seVolume_;
		char stereo_;
		char speaker_;
		char battleSpeed_;
		char button_;
		char reserved_;
		OptionStatus();
		~OptionStatus();
		static void initialize(OptionStatus* self);
		static void setSeVolume(OptionStatus* self, char id);
		static void setBattleSpeed(OptionStatus* self, char id);
		static void setSackSort(OptionStatus* self, char id);
		static char getSackSort(OptionStatus* self);
		static char getBgmVolume(OptionStatus* self);
		static char getSeVolume(OptionStatus* self);
		static void setSpeakerEnable(OptionStatus* self, char id);
		static char getSpeakerEnable(OptionStatus* self);
		static char getBattleSpeed(OptionStatus* self);
		static void setButton(OptionStatus* self, char id);
		static char getButton(OptionStatus* self);
		static void setBgmVolume(OptionStatus* self, char id);
	};
}