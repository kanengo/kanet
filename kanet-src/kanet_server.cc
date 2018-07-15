#include "kanet_server.h"

KanetServer::KanetServer()
{

}

KanetServer::~KanetServer()
{
    delete m_tp;
}

KanetServer* KanetServer::GetInstance()
{
    pthread_once(&KanetServer::ponce_, KanetServer::init);
    return m_instance;
}

void KanetServer::run()
{   
    InitIOThreads(2);
    InitWorkersThreads(8);
}

void KanetServer::InitServerSocket()
{

}

void KanetServer::InitIOThreads(int reactor_num)
{
    thread main_reactor(MainReactor);
}

void KanetServer::InitWorkersThreads(int worker_num)
{   
    if (worker_num == 0)
        worker_num = 1;
    m_tp = new ThreadPool(worker_num, 65536);
    m_tp->Detach();
}