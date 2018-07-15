#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <functional>

#include "socket_exception.h"
#include "socket_data.h"
#include "my_socket.h"
#include "inet_addr.h"
#include "socket_channel.h"
class MyEpoll
{
	typedef std::function<void(SocketChannle&)> callback;
	typedef epoll_event epEvent;
public:
	MyEpoll(int maxevents);
	void add(int fd, epEvent *event);
	void mod(int fd, epEvent *event);
	void del(int fd);
	int wait(epEvent *events, int maxevents, int timeout);

	void addListenFd(int fd);
	void addEvent(int fd, void* data);
	void modEvnet(int fd, void* data, bool write);
	
	// void setOnConnectCallback(callback cb);
	// void setOnReadCallback(callback cb);
	// void setWriteCallback(callback cb);
	void poll();
	virtual ~MyEpoll();
private:
	int efd_;
	int listenfd_;
	int maxevents_;
	epEvent *_events;
	
	
	// callback _onConnectCallback;
	// callback _onReadCallback;
	// callback _onWriteCallback;
	
};

