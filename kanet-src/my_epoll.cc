#include "my_epoll.h"


MyEpoll::MyEpoll(int maxevents = 64)
{
	efd_ = epoll_create(1024);
	if (efd_ == -1) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
	maxevents_ = maxevents;
	events_ = new epEvent[maxevents];
}

void MyEpoll::del(int fd)
{
	if (epoll_ctl(efd_, EPOLL_CTL_ADD, fd, nullptr) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void MyEpoll::add(int fd, epEvent * event)
{
	if (epoll_ctl(efd_, EPOLL_CTL_ADD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void MyEpoll::mod(int fd, epEvent * event)
{
	if (epoll_ctl(efd_, EPOLL_CTL_MOD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

int MyEpoll::wait(int timeout)
{
	return ::epoll_wait(efd_, events_, maxevents_, timeout);
}

void MyEpoll::addEvent(int fd, void* data)
{
	epEvent ev;
	ev.data.ptr = data;
	ev.events = EPOLLET | EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
	add(fd, &ev);
}


void MyEpoll::modEvnet(int fd, void * data, bool write)
{
	epEvent ev;
	ev.data.ptr = data;
	ev.events = EPOLLET | EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP|(write ? EPOLLOUT : 0);
	mod(fd, &ev);
}

// void MyEpoll::setOnConnectCallback(callback cb)
// {
// 	_onConnectCallback = cb;
// }

// void MyEpoll::setOnReadCallback(callback cb)
// {
// 	_onReadCallback = cb;
// }

// void MyEpoll::setWriteCallback(callback cb)
// {
// 	_onWriteCallback = cb;
// }


void MyEpoll::poll()
{
	/*int nevent;
	while (true)
	{
		nevent = wait(events_, maxevents_);
		for (int i = 0; i < nevent; i++)
		{
			auto data = static_cast<SocketData* >(events_[i].data.ptr);
			if (data->fd == listenfd_) {
				int  listenfd = data->fd;
				Socket s(listenfd);
				sockaddr_in clientaddr;
				Socket newSock = s.accept(&clientaddr);
				InetAddr addr(clientaddr);
				SocketChannle sc(newSock, addr);

				if (_onConnectCallback) {
					_onConnectCallback(sc);
				}
			}
			else if(events_[i].events & EPOLLIN){

			}
		}
	}*/
}


MyEpoll::~MyEpoll()
{
	if (efd_ > 0)
		close(efd_);
	if(events_)
		delete []events_;
}
