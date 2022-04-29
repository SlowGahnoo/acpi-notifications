#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <math.h>

class Mixer {
public:
	Mixer();
	~Mixer();

	/* Get the volume level in percentage */
	long getvolume(void);
	/* Set the volume level by percentage */
	void setvolume(long volume);
	/* Check if volume is muted */
	bool muted(void);

private:
	long min, max;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem;
	const char *card = "default";
	const char *selem_name = "Master";
};
