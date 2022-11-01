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

long logarithmic_scale(long volume, long max) 
{
	constexpr long N = 60;
	auto new_value = 100 + N * (log10(((double)volume + max * pow(10, (double) -100 / N)) / max));
	return (long) round(new_value);
}

long inverse_logarithmic_scale(long volume, long max)
{
	constexpr long N = 60;
	auto new_value = max * pow(10, (double)(volume - 100) / N) - max * pow(10, (double) -100 / N);
	return new_value;
}


long Mixer::getvolume(void) 
{
	long volume;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &volume);
	auto new_volume = logarithmic_scale(volume, max);
	return new_volume;
}

void Mixer::setvolume(long volume)
{
	auto new_volume = inverse_logarithmic_scale(volume, max);
	snd_mixer_selem_set_playback_volume_all(elem,  new_volume);
}

bool Mixer::muted(void)
{
	int mute;
	snd_mixer_selem_get_playback_switch(elem, SND_MIXER_SCHN_FRONT_LEFT, &mute);
	return (bool) mute;
}
