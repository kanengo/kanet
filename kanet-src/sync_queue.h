#pragma once
#include <atomic>
#include <vector>
// #include <list>
#include <condition_variable>
#include "lock.h"

using namespace std;

template<typename T>
class SyncQueue
{
public:
	static const uint8_t WRITE = 1;
	static const uint8_t READ = 2;
private:
	
	class QueueData {
	public:
		T data;
		uint8_t flag;
		QueueData() {
			flag = SyncQueue::WRITE;
		};
	};
	
public:
	explicit SyncQueue(int size = 1024)
		:capacity_(size)
	{
		count_ = 0;
		rdx_ = 0;
		wIdx_ = 0;
		m_q = new QueueData*[capacity_];
		for (int i = 0; i < capacity_; i++)
			m_q[i] = new QueueData();
		//m_q = new QueueData[capacity_];
	}

	SyncQueue(SyncQueue&& rhs)
	{
		capacity_ = rhs.capacity_;
		swap(m_q, rhs.m_q);
		count_ = 0;
		rdx_ = 0;
		wIdx_ = 0;
	}

	~SyncQueue()
	{
		for (int i = 0; i < capacity_; i++)
			delete m_q[i];
		delete[] m_q;
		//delete m_q;
	}
	
	bool push_and_wait(const T & ref) {
		LockGuard<Lock> lg(m_lock);
		while (full()) {
			cvfull.wait(m_lock);
		}
		_push(ref);
		return true;
	}

	bool push(const T & ref) 
	{
		LockGuard<Lock> lg(m_lock);
		auto ret = _push(ref);
		return ret;
		
	}

	T pop_and_wait() {
		LockGuard<Lock> lg(m_lock);
		while (empty()) {
			cvempty.wait(m_lock);
		}
		//auto ret = _pop();
		
		return std::ref(_pop());
	}

	T pop() {
		LockGuard<Lock> lg(m_lock);
		T &ret = _pop();
		return ret;
	}

	bool empty() const {
		return m_q[rdx_]->flag == SyncQueue::WRITE;
	}

	bool full() const {
		return m_q[wIdx_]->flag == SyncQueue::READ;
	}
private:

	bool _push(const T & ref) {
		if (!full()) {
			QueueData* node = m_q[wIdx_];
			node->flag = SyncQueue::READ;
			node->data = ref;
			wIdx_ = (wIdx_ + 1) % capacity_;
			count_ += 1;
			cvempty.notify_all();
			return true;
		}
		/*else if(m_qc.size() < m_cachesize){
			m_qc.push_back(ref);
		}*/

		return false;
	}

	T& _pop() {
		if (empty())
			cerr << "not value reutrn !!!";
		QueueData * node = m_q[rdx_];
		node->flag = SyncQueue::WRITE;
		rdx_ = (rdx_ + 1) % capacity_;
		count_ -= 1;
	/*	if (!m_qc.empty()) {
			_push(m_qc.front());
			m_qc.pop_front();
		}*/
		cvfull.notify_all();
		return node->data;

	}

	int capacity_;
	// int m_cachesize;
	QueueData ** m_q;
	//vector<QueueData> m_q;
	//list<T> m_qc;
	volatile int rdx_;
	volatile int wIdx_;
	volatile int count_;
	condition_variable_any cvempty;
	condition_variable_any cvfull;
	Lock m_lock;

};

