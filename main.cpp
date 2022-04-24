#include <iostream>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <libnotify/notification.h>
#include "Notification.h"


static const char *socket_path = "/var/run/acpid.socket";
static const int s_recv_len = 4096;

int main (int argc, char *argv[])
{
	notify_init("acpi");
	auto n = Volume();

	int sock = 0;
	int data_len = 0;
	char recv_msg[s_recv_len];
	struct sockaddr_un remote;
	memset(recv_msg, 0, s_recv_len * sizeof(char));

	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		std::cerr << "Error on socket() call\n";
		return 1;
	}

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, socket_path);
	data_len = strlen(remote.sun_path) + sizeof(remote.sun_family);

	puts("Connecting to socket...");
	if (connect(sock, (struct sockaddr*)&remote, data_len) == -1) {
		std::cerr << "Error on connect() call\n";
		return 1;
	}

	puts("Client connected");
	while (true) {
		if ((data_len = recv(sock, recv_msg, s_recv_len, 0)) > 0) {
			if (strncmp("button/volume", recv_msg, strlen("button/volume")) == 0) {
				n.update();
				n.show();
			}
		} else { 
			if (data_len < 0) {
				std::cerr << "Error on recv() call\n";
			} else {
				std::cerr << "Server socket closed\n";
				close(sock);
				break;
			}
		}
	}
	notify_uninit();
	return 0;
}
