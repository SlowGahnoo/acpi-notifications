#include "alsaaudio.h"

int Mixer::getvolume(void) 
{
	/* Get the volume level in percentage */
	long volume;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
	return (int)((double) volume / max * 100);
}

void Mixer::setvolume(long volume)
{
	/* Set the volume level by percentage */
	snd_mixer_selem_set_playback_volume_all(elem,  (long) ((double) volume * max / 100));
}

bool Mixer::muted(void)
{
	/* Check if volume is muted */
	int mute;
	snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &mute);
	return (bool) mute;
}
