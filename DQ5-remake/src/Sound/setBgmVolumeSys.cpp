
#include "Sound/setBgmVolumeSys.h"
#include "MenuPortSendSEVolume.h"
#include "MenuPortSendBGMVolume.h"

void Sound::setSeVolumeSys(int vol)
{
	MenuPortSendSEVolume(vol);
}

void Sound::setBgmVolumeSys(int vol)
{
	MenuPortSendBGMVolume(vol);
}

