#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <iostream>
#include "sync_queue.h"

using namespace std;
using handler = function<void()>;
class ThreadPool
{
public:
    ThreadPool(int poolSize, int qsize = 1024)
        : size_(poolSize), m_threads(vector<thread>()), m_queue(SyncQueue<handler>(qsize))
    {
        running_ = true;
        for (int i = 0; i < poolSize; i++)
        {
            m_threads.emplace_back([this]()
            {
                while(running_)
                {
                    auto task = m_queue.pop_and_wait();
                    task();
                }
            });
        }
    }

    void Detach()
    {
        for(std::size_t i = 0; i < m_threads.size(); i++)
        {
            if (m_threads[i].joinable())
                m_threads[i].detach();
        }
    }

    template<class F, class... Args>
    void Commit(F&& f, Args&&... args)
    {
        if(!running_)
            throw runtime_error("commit fail, thread pool is stopped");
        auto task = bind(forward<F>(f), forward<Args>(args)...);
        m_queue.push_and_wait(task);
        
    }

    ~ThreadPool()
    {   

        running_ = false;
        for(size_t i = 0; i < m_threads.size(); i++)
        {
            if(m_threads[i].joinable())
                m_threads[i].join();
        }

    }

private:
    int size_;
    bool running_;
    vector<thread> m_threads;
    SyncQueue<handler> m_queue;
    
};