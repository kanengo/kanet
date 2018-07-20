#pragma once
#include <thread>
#include <vector>
#include "thread_pool.h"
#include "my_epoll.h"

#include "nocopyable.h"

using namespace std;

class Kanet : NoCopyable
{
public:
    void start(int port);

    // static Kanet* GetInstance();
    Kanet();
    Kanet(int subIoThreadNum, int workerThreadNum);
    ~Kanet();


    // void listen(int port);

private:
    int subIoThreadNum_;
    int workerThreadNum_;
    // MySocket* listenSock_;

    static void SubReactor(MyEpoll& epoller);
    void InitIOThreads();
    void InitWorkersThreads();
    void MainReactor(int port);

};
