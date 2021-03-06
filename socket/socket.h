#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static const int s_recv_len = 4096;

class Socket {
public:
	Socket(const int __domain, const int __type, const int __protocol);
	~Socket();
	/* Connect to socket */
	int connect(const char *ip);
	/* Receive len bytes of data from socket (max is 4096) */
	char *recv(const size_t len);

private:
	int sock = 0;
	int data_len = 0;
	char recv_msg[s_recv_len];
};
