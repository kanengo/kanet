#include "socket_channel.h"

SocketChannle::SocketChannle(MySocket& s, InetAddr & addr)
	:socket_(s), inet_addr_(addr)
{
}

int SocketChannle::getSockFd()
{
	return socket_.sock_fd();
}

const MySocket &SocketChannle::socket() const
{
	return socket_;
}

const InetAddr &SocketChannle::inet_addr() const
{
	return inet_addr_;
}
