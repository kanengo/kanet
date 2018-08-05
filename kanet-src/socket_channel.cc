#include "socket_channel.h"

SocketChannle::SocketChannle(int fd, const sockaddr_in & addr)
{
	p_socket.reset(new MySocket(fd));
	p_inetAddr.reset(new InetAddr(addr));
}

SocketChannle::~SocketChannle()
{
}



int SocketChannle::getSockFd()
{
	return p_socket->sock_fd();
}

PSocket & SocketChannle::socket()
{
	return p_socket;
}

const PInetAddr & SocketChannle::inet_addr() const
{
	return p_inetAddr;
}
