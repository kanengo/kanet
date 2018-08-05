#include "kanet.h"

Kanet::Kanet()
{

}

Kanet::Kanet(int IOthreadNum)
    :IOthreadNum_(IOthreadNum)
{
}

Kanet::~Kanet()
{
    // delete m_tp;
    // delete listenSock_;
}

// Kanet* Kanet::GetInstance()
// {
//     // pthread_once(&Kanet::ponce_, Kanet::init);
//     static Kanet m_instance;
//     return &m_instance;
// }

void Kanet::start(int port)
{   
    // InitIOThreads();
    // InitWorkersThreads();
    // MainReactor(port);

    p_listensocket = make_shared<MySocket>(MySocket::CreateSocket());
    p_listensocket->setblocking(false);
    p_listensocket->setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    p_listensocket->setsockopt(SOL_SOCKET, SO_REUSEPORT, 1);
    p_listensocket->bind(port);
    p_listensocket->listen(64);

    int threadNum = IOthreadNum_ - 1;
    while(threadNum > 0)
    {
        // thread t(EventLoop, this);
    }

    EventLoop(this);

}

// MySocket Kanet::InitServerSocket()
// {

// }

void Kanet::EventLoop(Kanet* pKanet)
{
    MyEpoll epoller;
    epoller.addEvent(pKanet->p_listensocket->sock_fd());
    unordered_map<int, PChannle> clientMap;
    while(true)
    {   
        // epoll_event events[1024];
        int n = epoller.wait(-1);
        // int n = ::epoll_wait(epoller.efd(), events, 1024, -1);
        if (n <= 0)
            continue;
        auto events = epoller.events();
        for(int i = 0; i < n; i++)
        {
            if(events[i].data.fd == pKanet->p_listensocket->sock_fd())
            {
                sockaddr_in client_in;
                int client_fd = pKanet->p_listensocket->accept(&client_in);
                PChannle sc = make_shared<SocketChannle>(client_fd, client_in);
                sc->socket()->setblocking(false);
                clientMap[client_fd] = sc;
                epoller.addEvent(client_fd);
                cout << "accpet client:" << sc->socket()->sock_fd() <<", addr = " << sc->inet_addr()->getIp() << ", port = " << sc->inet_addr()->getPort() << endl;
                // cout << "accept ref count:" << sc.use_count() << endl;
            }
            else if(events[i].events & (EPOLLIN | EPOLLRDHUP | EPOLLPRI))
            {
                
                // SocketChannle* sc = static_cast<SocketChannle*>(events[i].data.ptr);
                // cout << "scoket fd = " << sc->socket()->sock_fd() << endl;
                // cout << "read ref count:" << clientMap[events[i].data.fd].use_count() << endl;
                PChannle channle = clientMap[events[i].data.fd];
                char buf[1024];
                int n = 0;
                bool bError = false;
                while(true)
                {
                    auto rszie = channle->socket()->recv(buf + n, 1024, 0);
                    cout << "recv size:" << rszie << ", fd:"<< channle->getSockFd() <<endl;
                    if (rszie == 0)
                    {
                        // cout << "client close:" << channle->socket()->sock_fd() << endl;
                        epoller.del(channle->socket()->sock_fd());
                        clientMap.erase(channle->socket()->sock_fd());
                        channle->socket()->close();
                        bError = true;
                        break;
                    }
                    else if(rszie == -1)
                    {
                        if(errno == EINTR || errno == EAGAIN)
                            break;
                        cout << "client close exception:" << channle->socket()->sock_fd() << endl;
                        epoller.del(channle->socket()->sock_fd());
                        clientMap.erase(channle->socket()->sock_fd());
                        channle->socket()->close();
                        bError = true;
                        break;
                    }
                    else
                    {
                        n += rszie;
                    }
                }
                if (!bError)
                {
                    // cout << "recv:" << buf << endl;
                    // string replay("Men can’t wait but they always say “I’ll spend my life to wait for you”. Women can’t wait either, but they wait for the whole life. 男人等不了，却时常说“等你一辈子”。女人等不起，却等了一辈子。");
                    // channle->socket()->send(replay.c_str(), replay.size() + 1, 0);
                    epoller.modEvnet(channle->getSockFd(), true);
                }
            }
            else if (events[i].events & EPOLLOUT)
            {   
                cout << "111111111111" << endl;
                PChannle channle = clientMap[events[i].data.fd];
                string replay("Men can’t wait but they always say “I’ll spend my life to wait for you”. Women can’t wait either, but they wait for the whole life. 男人等不了，却时常说“等你一辈子”。女人等不起，却等了一辈子。");
                auto ns = replay.size() + 1;
                while(true)
                {
                    auto size = channle->socket()->send(replay.c_str(), replay.size() + 1, 0);
                    if(size == -1)
                    {
                        if(errno == EAGAIN || errno == EINTR)
                            continue;
                        break;
                    }
                    ns -= size;
                    if (ns <= 0)
                        break;
                }
                cout << "22222222222222" << endl;
                // SocketChannle* sc = static_cast<SocketChannle*>(events[i].data.ptr);
            }
        }
    }
}

void Kanet::InitWorkersThreads()
{   

}

// void Kanet::listen(int port)
// {
//     listenSock_ = new MySocket(std::move(MySocket::CreateSocket(AF_INET, SOCK_STREAM, 0)));
//     listenSock_->bind(port);
//     listenSock_->listen(64);
// }

void Kanet::MainReactor(int port)
{
    MySocket listenSock = MySocket::CreateSocket();
    listenSock.setblocking(false);
    listenSock.bind(port);
    listenSock.listen(64);

    MyEpoll mainEpoller(64);
    mainEpoller.addEvent(listenSock.sock_fd());

    vector<PEoll> epollers;
    vector<thread> subIOer;
    // if(subIoThreadNum_ > 0)
    // {
    //     for (int i = 0; i < subIoThreadNum_; i++)
    //     {
    //         // MyEpoll epoller = MyEpoll(64);
    //         MyEpollPtr epoller = make_shared<MyEpoll>(64);
    //         cout << "sub reactor epoll fd:" << epoller->efd() << endl;
    //         epollers.emplace_back(epoller);
    //     }
    //     for (int i = 0; i < epollers.size(); i++){
    //         subIOer.emplace_back([](int efd)
    //         {   
    //             SubReactor(efd);
    //         }, epollers[i]->efd());
    //     }
    // }

    int index = 0;
    cout << "---------------start kanet server------------------" << mainEpoller.efd() <<endl;
    while(true)
    {   
        int n = mainEpoller.wait(-1);
        auto events = mainEpoller.events();
        for(int i = 0; i < n; i++)
        {
            if(events[i].data.fd == listenSock.sock_fd())
            {
                sockaddr_in client_in;
                int client_fd = listenSock.accept(&client_in);
                PChannle sc = make_shared<SocketChannle>(client_fd, client_in);
                epollers[index]->addEvent(client_fd, &sc);
                // mainEpoller.addEvent(client_fd, &sc);
                index += 1;
                index %= epollers.size();
                cout << "accpet client:" << sc->socket()->sock_fd() <<", addr = " << sc->inet_addr()->getIp() << endl;
            }
            else
            {
                cout << "main reactor error" << endl;
                exit(2);
            }
        }
    }
}
 
            // if(events[i].events & (EPOLLERR))
            // {
            //     PChannle sc = *(static_cast<PChannle*>(events[i].data.ptr));
            //     epoller.del(sc->socket().sock_fd());
            //     sc->socket().close();
            // }
            // else if (events[i].events & (EPOLLIN | EPOLLRDHUP | EPOLLPRI))
            // {
            //     PChannle sc = *(static_cast<PChannle*>(events[i].data.ptr));
            //     char buf[1024];
            //     auto rszie = sc->socket().recv(buf, 1024, 0);
            //     if (rszie <= 0)
            //     {
            //         cout << "client close:" << sc->socket().sock_fd() << endl;
            //         epoller.del(sc->socket().sock_fd());
            //         sc->socket().close();
            //     }
            //     else
            //     {
            //         cout << "recv:" << buf << endl;
            //     }
            // }
            // else if(events[i].events & (EPOLLOUT))
            // {

            // }

