#include <iostream>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <string.h>
#include <unistd.h>

#include "notification/notification.h"
#include "socket/socket.h"

static const char *socket_path = "/var/run/acpid.socket";

#define STRLEN(x) (sizeof(x) - 1)
#define STREQ(x, y) ((strncmp((x), y, STRLEN(x)) == 0))

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	auto app = NotificationApplication("acpi");
	auto n = Volume();
	auto b = Backlight();
	auto s = Socket(AF_UNIX, SOCK_STREAM, 0);
	s.connect(socket_path);
	while (true) {
		char *recv_msg = s.recv(4096);
		if (STREQ("button/volume", recv_msg)) {
			n.volume(recv_msg[STRLEN("button/volume")]);
			n.update();
			n.show();
		} else if (STREQ("button/mute", recv_msg)) {
			n.toggle();
			n.show();
		}
		if (STREQ("video/brightness", recv_msg)) {
			b.set(recv_msg[STRLEN("video/brightness")]);
			b.update();
			b.show();
		}
	}
	return 0;
}
