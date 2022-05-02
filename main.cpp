#include <iostream>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <string.h>
#include <unistd.h>

#include "notification/notification.h"
#include "socket/socket.h"

static const char *socket_path = "/var/run/acpid.socket";

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	notify_init("acpi");
	auto n = Volume();
	auto b = Backlight();
	auto s = Socket(AF_UNIX, SOCK_STREAM, 0);
	s.connect(socket_path);
	while (true) {
		char *recv_msg = s.recv(4096);
		if (strncmp("button/volume", recv_msg, strlen("button/volume")) == 0) {
			n.volume(recv_msg[strlen("button/volume")]);
			n.update();
			n.show();
		} else if (strncmp("button/mute", recv_msg, strlen("button/mute")) == 0) {
			n.toggle();
			n.show();
		}

		if(strncmp("video/brightness", recv_msg, strlen("video/brightness")) == 0) {
			b.set(recv_msg[strlen("video/brightness")]);
			b.update();
			b.show();
		}
	}
	notify_uninit();
	return 0;
}
