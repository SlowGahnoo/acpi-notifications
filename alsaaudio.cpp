#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

#ifndef __ALSAAUDIO_H__
#define __ALSAAUDIO_H__

class Mixer {
public:
	Mixer() {
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

	~Mixer() {
		snd_mixer_close(handle);
	}

	int getvolume(void);
	void setvolume(long volume);
	bool muted(void);

private:
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t* elem;
    const char *card = "default";
    const char *selem_name = "Master";
};

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
	snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);
}

bool Mixer::muted(void)
{
	/* Check if volume is muted */
	return false;
}

#endif
