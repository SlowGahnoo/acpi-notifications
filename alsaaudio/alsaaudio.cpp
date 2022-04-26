#include "alsaaudio.h"

long Mixer::getvolume(void) 
{
	long volume;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
	return (long) round((double) volume / max * 100);
}

void Mixer::setvolume(long volume)
{
	snd_mixer_selem_set_playback_volume_all(elem,  (long) round((double) volume * max / 100));
}

bool Mixer::muted(void)
{
	int mute;
	snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &mute);
	return (bool) mute;
}
