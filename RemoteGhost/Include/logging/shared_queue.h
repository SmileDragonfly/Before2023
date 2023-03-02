#ifndef SHARED_QUEUE
#define SHARED_QUEUE

#if (_MSC_VER >= 1700)

#include <queue>
#include <mutex>
#include <exception>
#include <condition_variable>

/** Multiple producer, multiple consumer thread safe queue
* Since 'return by reference' is used this queue won't throw */
template<typename T>
class shared_queue
{
	std::queue<T> queue_;
	mutable std::mutex m_;
	std::condition_variable data_cond_;

	shared_queue& operator=(const shared_queue&) = delete;
	shared_queue(const shared_queue& other) = delete;

public:
	shared_queue(){}

	void push(T item){
		std::lock_guard<std::mutex> lock(m_);
		queue_.push(item);
		data_cond_.notify_one();
	}

	/// \return immediately, with true if successful retrieval
	bool try_and_pop(T& popped_item){
		std::lock_guard<std::mutex> lock(m_);
		if (queue_.empty()){
			return false;
		}
		popped_item = queue_.front();
		queue_.pop();
		return true;
	}

	/// Try to retrieve, if no items, wait till an item is available and try again
	bool wait_and_pop(T& popped_item){
		std::unique_lock<std::mutex> lock(m_); // note: unique_lock is needed for std::condition_variable::wait
		while (queue_.empty())
		{ //                       The 'while' loop below is equal to
			data_cond_.wait(lock);  //data_cond_.wait(lock, [](bool result){return !queue_.empty();});
		}
		popped_item = queue_.front();
		queue_.pop();
		return true;
	}

	bool empty() const{
		std::lock_guard<std::mutex> lock(m_);
		return queue_.empty();
	}

	unsigned size() const{
		std::lock_guard<std::mutex> lock(m_);
		return queue_.size();
	}
};

#else

#include <queue>
#include <exception>
#include "OSSpecific.h"
#include <afxmt.h>

/** Multiple producer, multiple consumer thread safe queue
* Since 'return by reference' is used this queue won't throw */
template<typename T>
class shared_queue
{
	std::queue<T> queue_;
	//mutable std::mutex m_;
	//CMutex m_;
	CCriticalSection m_;

	//std::condition_variable data_cond_;
	OSSpecific::ConditionT data_cond_;

	shared_queue& operator=(const shared_queue&); // c++11 feature not yet in vs2010 = delete;
	shared_queue(const shared_queue& other); // c++11 feature not yet in vs2010 = delete;

public:
	shared_queue(){ OSSpecific::CreateCondition(&data_cond_); }

	void push(T item){
		{
			//std::lock_guard<std::mutex> lock(m_);
			CSingleLock locker(&m_);
			locker.Lock();

			queue_.push(item);
		}
		//data_cond_.notify_one();
		OSSpecific::SignalCondition(data_cond_);
  }

	/// \return immediately, with true if successful retrieval
	bool try_and_pop(T& popped_item){
		//std::lock_guard<std::mutex> lock(m_);
		CSingleLock locker(&m_);
		locker.Lock();

		if(queue_.empty()){
			return false;
		}
		popped_item=queue_.front();
		queue_.pop();
		return true;
	}

	/// Try to retrieve, if no items, wait till an item is available and try again
	bool wait_and_pop(T& popped_item){
		//std::unique_lock<std::mutex> lock(m_); // note: unique_lock is needed for std::condition_variable::wait
		CSingleLock locker(&m_);
		locker.Lock();
		while(queue_.empty())
		{ //                       The 'while' loop below is equal to
			//data_cond_.wait(lock);  //data_cond_.wait(lock, [](bool result){return !queue_.empty();});
			locker.Unlock();
			OSSpecific::WaitForCondition(data_cond_);
			return false;
		}
		//popped_item=std::move(queue_.front());
		popped_item = queue_.front();
		queue_.pop();
		return true;
	}

	bool empty() const{
		//std::lock_guard<std::mutex> lock(m_);
		CSingleLock locker(&m_);
		locker.Lock();
		return queue_.empty();
	}

	unsigned int size() {
		//std::lock_guard<std::mutex> lock(m_);
		CSingleLock locker(&m_);
		locker.Lock();
		return (int) queue_.size();
	}
};
#endif //(_MSC_VER >= 1700)


#endif //SHARED_QUEUE