#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include "thread_pool.h"
#include "my_epoll.h"
#include <unordered_map>

#include "nocopyable.h"

using namespace std;
typedef shared_ptr<MyEpoll>  PEoll;
typedef shared_ptr<SocketChannle> PChannle;
typedef shared_ptr<MySocket> PSocket;
class Kanet
{
public:
    void start(int port);

    // static Kanet* GetInstance();
    Kanet();
    Kanet(int IOthreadNum);
    ~Kanet();


    // void listen(int port);

private:
    int IOthreadNum_ = 1;
    int workerThreadNum_ = 1;
    PSocket p_listensocket;

    // MySocket* listenSock_;

    static void SubReactor(int efd);
    void InitIOThreads();
    static 
    void EventLoop(Kanet* pKanet);
    void InitWorkersThreads();
    void MainReactor(int port);

};
