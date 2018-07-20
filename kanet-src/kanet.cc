#include "kanet.h"

Kanet::Kanet()
{
    Kanet(1,1);
}

Kanet::Kanet(int subIoThreadNum, int workerThreadNum)
    :subIoThreadNum_(subIoThreadNum),workerThreadNum_(subIoThreadNum)
{
     InitIOThreads();
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
    MainReactor(port);
}

// MySocket Kanet::InitServerSocket()
// {

// }

void Kanet::InitIOThreads()
{

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
    MySocket listenSock = MySocket::CreateSocket(AF_INET, SOCK_STREAM, 0);
    MyEpoll main_epoll(64);
    main_epoll.addEvent(listenSock.sock_fd(), nullptr);
    vector<MyEpoll> epollers;
    vector<thread> subIOer;
    if(subIoThreadNum_ > 0)
    {
        for (int i = 0; i < subIoThreadNum_; i++)
        {
            MyEpoll epoller = MyEpoll(64);
            epollers.emplace_back(epoller);
            subIOer.emplace_back([&epoller]()
            {
                SubReactor(epoller);
            });
        }
    }

    while(true)
    {   
        int n = main_epoll.wait(-1);
        auto events = main_epoll.events();
        for(int i = 0; i < n; i++)
        {
            if(events[i].data.fd == listenSock.sock_fd())
            {
                sockaddr_in client_in;
                auto client_sock = listenSock.accept(&client_in);
            }
            else
            {
                exit(2);
            }
        }
    }
}

void Kanet::SubReactor(MyEpoll& epoller)
{   
    while(true)
    {
        int n = epoller.wait(-1);
        auto events = epoller.events();
        for(int i = 0; i < n; i++)
        {
            if (events[i].events == EPOLLIN)
            {
                
            }
        }
    }
}