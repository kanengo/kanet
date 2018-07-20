#include "my_socket.h"

MySocket::MySocket(MySocket&& rhs)
{
	sock_fd_ = rhs.sock_fd_;
}

MySocket MySocket::CreateSocket(int protofamily = AF_INET, int type = SOCK_STREAM, int protocol = 0)
{
	int sockfd = ::socket(protofamily, type, protocol);
	if (sockfd <= 0) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
	
	return MySocket(sockfd);
}

void MySocket::setsockopt(int level, int optname, int optval)
{
	if (::setsockopt(sock_fd_, level, optname, &optval, static_cast<socklen_t>(sizeof(optval))) == -1) {
		throw SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

void MySocket::setsockopt(int level, int optname, const void *optval, socklen_t optsize)
{
	if (::setsockopt(sock_fd_, level, optname, optval, optsize) == -1) {
		throw SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

bool MySocket::bind(const uint16_t port)
{
	addrinfo ai_hints;
	addrinfo *ai_res = nullptr;
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_protocol = IPPROTO_TCP;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_PASSIVE;
	//char strPort[6];
	//sprintf(strPort, "%d", port);
	if (getaddrinfo("0.0.0.0", std::to_string(port).c_str(), &ai_hints, &ai_res) != 0) {
		throw SocketException("function 'getaddrinfo' error:" + std::string(strerror(errno)));
	}
	if (::bind(sock_fd_, ai_res->ai_addr, sizeof(sockaddr)) != 0) {
		throw SocketException("socket bind error:" + std::string(strerror(errno)));
	}

	return true;
}

bool MySocket::listen(const int maxListenNum)
{
	if (::listen(sock_fd_, maxListenNum) != 0) {
		throw SocketException("socket listen error:" + std::string(strerror(errno)));
	}
	return true;
}

bool MySocket::setblocking(const bool isBlock)
{
	int flag = fcntl(sock_fd_, F_GETFL, 0);
	if (isBlock)
		flag |= O_NONBLOCK;
	else
		flag &= O_NONBLOCK;
	if (fcntl(sock_fd_, F_SETFL, flag) == -1) {
		throw SocketException("set socket blocking error:" + std::string(strerror(errno)));
	}

	return true;
}

bool MySocket::connect(const std::string host, const uint16_t port)
{	
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &serveraddr.sin_addr);
	if (::connect(sock_fd_, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		throw SocketException("socket connect error:" + std::string(strerror(errno)));
	}
	return true;
}

ssize_t MySocket::recv(void * buf, size_t sz, int flag = 0)
{
	//size_t left = sz;
	//ssize_t nread = 0;
	//char* p = static_cast<char *>(buf);
	//while (left > 0) {
	//	nread = ::recv(sock_fd_, p, left, flag);
	//	if (nread == 0) {
	//		break;
	//	}
	//	else if (nread < 0) {
	//		if (errno == EINTR)
	//			continue;
	//		return -1;
	//	}
	//	else {
	//		left -= nread;
	//		p += nread;
	//	}
	//}
	//
	//return sz - left;
	ssize_t nread = ::recv(sock_fd_, buf, sz, flag);
	return nread;
	
}

ssize_t MySocket::send(const void * buf, size_t len, int flag = 0)
{
	return ::send(sock_fd_, buf, len, flag);
}

MySocket MySocket::accept(sockaddr_in* clientaddr = nullptr)
{
	socklen_t socklen = static_cast<socklen_t>(sizeof(sockaddr));
	int client_fd = ::accept(sock_fd_, (sockaddr*)clientaddr, &socklen);
	if (client_fd == -1) {
		throw SocketException("socket accept error:" + std::string(strerror(errno)));
	}

	MySocket s(client_fd);
	return s;
}

bool MySocket::close()
{
	if (sock_fd_ > 0)
		::close(sock_fd_);
	else
		return false;
	sock_fd_ = -1;
	return true;
}

int MySocket::sock_fd() const
{
	return sock_fd_;
}


MySocket::~MySocket()
{
	close();
}

bool MySocket::isVaild()
{
	if (sock_fd_ <= 0)
		return false;
	return true;
}
