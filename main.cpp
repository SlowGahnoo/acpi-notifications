#include <iostream>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <libnotify/notification.h>
#include <libnotify/notify.h> 
#include "notification/notification.h"


static const char *socket_path = "/var/run/acpid.socket";
static const int s_recv_len = 4096;

class Socket {
public:
	Socket(int __domain, int __type, int __protocol) {
		sock = socket(__domain, __type, __protocol);
		memset(recv_msg, 0, s_recv_len * sizeof(char));

		if ((sock = socket(__domain, __type, 0)) == -1) {
			std::cerr << "Error on socket() call\n";
			exit(1);
		}

	}

	int connect(const char *ip) {
		puts("Connecting to socket...");
		struct sockaddr_un addr;
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, ip, sizeof(addr.sun_path) - 1);
		int status = ::connect(sock, (struct sockaddr*)&addr, sizeof(addr));
		if (status < 0) {
			std::cerr << "Error on connect() call\n";
			exit(1);
		}
		puts("Client connected");
		return status;
	}

	char *recv() {
		if ((data_len = ::recv(sock, recv_msg, s_recv_len, 0)) > 0) {
			return recv_msg;
		} else {
			if (data_len < 0) {
				std::cerr << "Error on recv() call\n";
			} else {
				std::cerr << "Server socket closed\n";
				close(sock);
				exit(1);
			}
			return NULL;
		}
	}

private:
	int sock = 0;
	int data_len = 0;
	char recv_msg[s_recv_len];
};

int main (int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	notify_init("acpi");
	auto n = Volume();
	auto s = Socket(AF_UNIX, SOCK_STREAM, 0);
	s.connect(socket_path);
	while (true) {
		char *recv_msg = s.recv();
			if (strncmp("button/volume", recv_msg, strlen("button/volume")) == 0) {
				n.volume(recv_msg[strlen("button/volume")]);
				n.update();
				n.show();
			} else if (strncmp("button/mute", recv_msg, strlen("button/mute")) == 0) {
				n.toggle();
				n.show();
			}
	}
	notify_uninit();
	return 0;
}
