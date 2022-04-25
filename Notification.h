#include <cstdint>
#include <iostream>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <fmt/core.h>
#include <random>
#include <math.h>


#include "alsaaudio.cpp"

#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

class ProgressBar {
public:
	ProgressBar(size_t sz) : sz(sz) {
	}

	std::string getProgressString(int status) {
		auto filled = (int)((double) sz * status / 100 + 0.5);
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

class Notification {
public:
	Notification() : title(""), desc(""), icon(""){
		n = notify_notification_new("", "", "");
		g_object_set (G_OBJECT (n), 
				"id",        rand(),
				"summary",   title.c_str(),
				"body",      desc.c_str(),
				"icon-name", icon, NULL
				);
	}

	void updateParam() {
		g_object_set (G_OBJECT (n), 
				"summary",   title.c_str(),
				"body",      desc.c_str(),
				"icon-name", icon, NULL
				);
	}

	~Notification() {
		g_object_unref(G_OBJECT (n));
	}

	void show(void) {
		notify_notification_show(n, NULL);
	}
protected:
	NotifyNotification *n;
	std::string title;
	std::string desc;
	const char *icon;
};

class Volume : public Notification {
public:
	Volume() : Notification(), b(30) {
		this->update();
	}

	void update() {
		auto m = Mixer();
		auto volume = m.getvolume();
		this->title = fmt::format("Volume level: {}%", volume);
		this->desc = b.getProgressString(volume);
		this->icon = icons[(int)ceil((double) 0.03 * volume)];
		updateParam();
	}

private:
	const char *icons[4] = {
		    "audio-volume-muted",
		    "audio-volume-low",
		    "audio-volume-medium",
		    "audio-volume-high" 
	};

	ProgressBar b;
	Mixer m;
};

#endif
