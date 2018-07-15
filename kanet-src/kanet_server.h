#pragma once
#include <thread>
#include <vector>
#include "thread_pool.h"
#include "my_epoll.h"

#include "nocopyable.h"

class KanetServer
{
public:
    void run();

    static KanetServer* GetInstance();
    

private:
    KanetServer();
    ~KanetServer();
    void InitIOThreads(int reactor_num);
    void InitWorkersThreads(int worker_num);
    void InitServerSocket();
    void MainReactor();
    void SubReacotr();

    static void init()
    {
        m_instance = new KanetServer();
    }

    vector<MyEpoll> m_eps;
    ThreadPool *m_tp;


    static pthread_once_t ponce_;
    static KanetServer* m_instance;

};
pthread_once_t KanetServer::ponce_ = PTHREAD_ONCE_INIT;
KanetServer* KanetServer::m_instance = nullptr;

