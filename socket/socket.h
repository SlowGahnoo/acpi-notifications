#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

static const int s_recv_len = 4096;

class Socket {
public:
	Socket(const int __domain, const int __type, const int __protocol) {
		sock = socket(__domain, __type, __protocol);
		memset(recv_msg, 0, s_recv_len * sizeof(char));

		if ((sock = socket(__domain, __type, 0)) == -1) {
			std::cerr << "Error on socket() call\n";
			exit(1);
		}

	}

	~Socket() {
		close(sock);
	}

	int connect(const char *ip);
	char *recv(const size_t len);

private:
	int sock = 0;
	int data_len = 0;
	char recv_msg[s_recv_len];
};
