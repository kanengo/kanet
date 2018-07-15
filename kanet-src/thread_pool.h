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
        : m_size(poolSize), m_threads(vector<thread>()), m_queue(SyncQueue<handler>(qsize))
    {
        m_running = true;
        for (int i = 0; i < poolSize; i++)
        {
            m_threads.emplace_back([&]()
            {
                while(m_running)
                {
                    auto task = m_queue.pop_and_wait();
                    task();
                }
            });
        }
    }

    template<class F, class... Args>
    void commit(F&& f, Args&&... args)
    {
        if(!m_running)
            throw runtime_error("commit fail, thread pool is stopped");
        auto task = bind(forward<F>(f), forward<Args>(args)...);
        m_queue.push_and_wait(task);
        
    }

    ~ThreadPool()
    {   

        m_running = false;
        for(size_t i = 0; i < m_threads.size(); i++)
        {
            if(m_threads[i].joinable())
                m_threads[i].join();
        }

    }

  private:
    int m_size;
    bool m_running;
    vector<thread> m_threads;
    SyncQueue<handler> m_queue;
    
};