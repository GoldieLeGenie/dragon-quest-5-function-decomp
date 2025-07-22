#include "status/OptionStatus.h"
#include "Sound/setBgmVolumeSys.h"


status::OptionStatus::OptionStatus()
{
	this->sackSort_ = 0x40400;
	this->speaker_ = 0x200;
	this->button_ = 2;
}


status::OptionStatus::~OptionStatus()
{
	;
}

void status::OptionStatus::initialize(OptionStatus* self)
{
	self->bgmVolume_ = 4;
	Sound::setBgmVolumeSys(127);
	self->seVolume_ = 4;
	Sound::setSeVolumeSys(127);
	self->battleSpeed_ = 2;
}

void status::OptionStatus::setBgmVolume(OptionStatus* self, char id)
{
	self->bgmVolume_ = id;
	Sound::setBgmVolumeSys((127 * id + 127) / 5);
}

void status::OptionStatus::setSeVolume(OptionStatus* self, char id)
{
	self->seVolume_ = id;
	Sound::setSeVolumeSys((127 * id + 127) / 5);
}

void status::OptionStatus::setBattleSpeed(OptionStatus* self, char id)
{
	self->battleSpeed_ = id;
}

void status::OptionStatus::setSackSort(OptionStatus* self, char id)
{
	self->sackSort_ = id;
}

char status::OptionStatus::getSackSort(OptionStatus* self)
{
	return self->sackSort_;
}

char status::OptionStatus::getBgmVolume(OptionStatus* self)
{
	return self->bgmVolume_;
}

char status::OptionStatus::getSeVolume(OptionStatus* self)
{
	return self->seVolume_;
}

void status::OptionStatus::setSpeakerEnable(OptionStatus* self, char id)
{
	self->stereo_ = id;
}

char status::OptionStatus::getSpeakerEnable(OptionStatus* self)
{
	return self->stereo_;
}

char status::OptionStatus::getBattleSpeed(OptionStatus* self)
{
	return self->battleSpeed_;
}

void status::OptionStatus::setButton(OptionStatus* self, char id)
{
	self->button_ = id;
}

char status::OptionStatus::getButton(OptionStatus* self)
{
	return self->button_;
}

