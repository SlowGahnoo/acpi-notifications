#include "alsaaudio.h"

Mixer::Mixer()
{
	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, card);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	elem = snd_mixer_find_selem(handle, sid);
	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
}

Mixer::~Mixer()
{
	snd_mixer_close(handle);
}


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
