#pragma once
#include "my_socket.h"
#include "inet_addr.h"
#include <memory>
#include <iostream>

typedef std::shared_ptr<MySocket> PSocket;
typedef std::shared_ptr<InetAddr> PInetAddr;

class SocketChannle
{
public:
	SocketChannle(int fd, const sockaddr_in & addr);
	~SocketChannle();
	int getSockFd();
	PSocket& socket();
	const PInetAddr & inet_addr() const;

private:
	PSocket p_socket;
	PInetAddr p_inetAddr;
};

