#pragma once
#include <sys/socket.h>
#include <cerrno>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "inet_addr.h"
#include "socket_exception.h"

class MySocket
{
public:

	MySocket(int sockfd) :sock_fd_(sockfd) {};
	MySocket(MySocket&& rhs);
	static MySocket CreateSocket(int protofamily, int type, int protocol);
	void setsockopt(int level, int optname, int optval);
	void setsockopt(int level, int optname, const void *optval, socklen_t optsize);
	bool bind(const uint16_t port);
	bool listen(const int maxListenNum);
	bool setblocking(const bool isBlock);
	bool connect(const std::string host, const uint16_t port);
	ssize_t recv(void* buf, size_t len, int flag);
	ssize_t send(const void *buf, size_t len, int flag);
	MySocket accept(sockaddr_in* clientaddr);
	bool close();
	int sock_fd() const;
	~MySocket();

	
private:
	bool isVaild();
	int sock_fd_;
};

