#pragma once
#include "my_socket.h"
#include "inet_addr.h"

class SocketChannle
{
public:
	SocketChannle(MySocket&s, InetAddr &addr);
	int getSockFd();
	const MySocket& socket() const;
	const InetAddr & inet_addr() const;
private:
	MySocket socket_;
	InetAddr inet_addr_;
};

