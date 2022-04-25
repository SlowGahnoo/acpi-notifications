#include "socket.h"

int Socket::connect(const char *ip)
{
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, ip, sizeof(addr.sun_path) - 1);
	int status = ::connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (status < 0) {
		std::cerr << "Error on connect() call\n";
		exit(1);
	}
	return status;
}

char *Socket::recv(const size_t len)
{
	if ((data_len = ::recv(sock, recv_msg, len, 0)) > 0) {
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
