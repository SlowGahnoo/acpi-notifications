#include <cstdint>
#include <fmt/core.h>
#include <glib.h>
#include <gsound.h>
#include <iostream>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <math.h>
#include <glib.h>
#include <gsound.h>
#include <fstream>
#include <random>

#include "../alsaaudio/alsaaudio.h"

static GSoundContext *sound_context = gsound_context_new(NULL, NULL);

class ProgressBar {
public:
	ProgressBar(size_t sz) : sz(sz)
	{
	}

	std::string getProgressString(int status)
	{
		auto filled = (int)((double)sz * status / 100 + 0.5);
		std::string p1 = "";
		for (auto i = 0; i < filled; i++) {
			p1 += "▄";
		}
		for (auto i = 0; i < sz - filled; i++) {
			p1 += "▁";
		}
		return p1;
	}

private:
	int sz;
};

class NotificationApplication {
public:
	const char *app_name;
	NotificationApplication(const char *app_name) : app_name(app_name) {
		notify_init(app_name);
	}
	~NotificationApplication() {
		notify_uninit();
	}
private:
};

class Notification {
public:
	Notification() : title(""), desc(""), icon("")
	{
		n = notify_notification_new("", "", "");
		g_object_set(G_OBJECT(n), "id", rand(), "summary",
			     title.c_str(), "body", desc.c_str(), "icon-name",
			     icon, NULL);
	}

	~Notification()
	{
		g_object_unref(G_OBJECT(n));
	}

	/* Update parameters of notification gobject */
	void updateParam()
	{
		g_object_set(G_OBJECT(n), "summary", title.c_str(), "body",
			     desc.c_str(), "icon-name", icon, NULL);
	}

	/* Send and show the notification */
	void show(void)
	{
		gsound_context_play_simple(sound_context, NULL, NULL,
					   GSOUND_ATTR_EVENT_ID, sound, NULL);
		notify_notification_show(n, NULL);
	}

protected:
	NotifyNotification *n;
	std::string title;
	std::string desc;
	const char *icon;
	const char *sound;
};

class Volume : public Notification {
public:
	Volume() : Notification(), b(30)
	{
		this->sound = "audio-volume-change";
		this->update();
	}

	/* Increment/Decrement volume by 5% based on a flag
	 * (inc = 'u', dec = 'd') */
	void volume(int flag)
	{
		auto m = Mixer();
		auto volume = m.getvolume();
		auto new_val = 0;
		switch (flag) {
		case 'u':
			new_val = volume + 5;
			volume = (new_val < 100) ? new_val : 100;
			break;
		case 'd':
			new_val = volume - 5;
			volume = (new_val > 0) ? new_val : 0;
			break;
		default:
			std::cerr << "Unknown flag passed: '" << flag << "'\n";
			break;
		}
		m.setvolume(volume);
	}

	/* Update notification's data */
	void update()
	{
		auto m = Mixer();
		auto volume = m.getvolume();
		this->title = fmt::format("Volume level: {}%", volume);
		this->desc = b.getProgressString(volume);
		this->icon = icons[(int)ceil((double)0.03 * volume)];
		updateParam();
	}

	/* Mute/Unmute audio */
	void toggle()
	{
		bool muted = Mixer().muted();
		if (muted) {
			this->title = "Volume level: Muted";
			this->icon = this->icons[0];
			updateParam();
		} else {
			this->update();
		}
	}

private:
	const char *icons[4] = { "audio-volume-muted", "audio-volume-low",
				 "audio-volume-medium", "audio-volume-high" };

	ProgressBar b;
	Mixer m;
};

class Backlight : public Notification {
public:
	Backlight() : Notification(), b(30)
	{
		std::ifstream(max_path) >> max;
		std::ifstream(level_path) >> level;
		perc = round(100 * level / max);
		this->sound = "audio-volume-change";
	}

	void set(int flag)
	{
		long val = (long)round(perc);
		long new_val = 0;
		std::ofstream brightness(level_path);
		switch (flag) {
		case 'u':
			new_val = val + 5;
			val = (new_val < 100) ? new_val : 100;
			break;
		case 'd':
			new_val = val - 5;
			val = (new_val > 0) ? new_val : 0;
			break;
		default:
			std::cerr << "Unknown flag passed: '" << flag << "'\n";
			break;
		}
		brightness << (long)round((double)(new_val * max / 100));
	}

	void update()
	{
		std::ifstream(max_path) >> max;
		std::ifstream(level_path) >> level;
		perc = round(100 * level / max);
		this->title = fmt::format("Backlight level: {}%", perc);
		this->desc = b.getProgressString(perc);
		this->icon = icons[int(perc / 25)];
		this->updateParam();
	}

private:
	const char *icons[5] = { "notification-display-brightness-low",
				 "notification-display-brightness-medium",
				 "notification-display-brightness-high",
				 "notification-display-brightness-full",
				 "notification-display-brightness-full" };

	const char *level_path =
		"/sys/class/backlight/intel_backlight/brightness";
	const char *max_path =
		"/sys/class/backlight/intel_backlight/max_brightness";
	double level, max;
	double perc;
	ProgressBar b;
};
