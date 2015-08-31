#pragma once
#include <windows.h>
#include <queue>
#ifdef _MSC_VER
#pragma warning(disable:4018)
#endif

using namespace std;
template <typename T>

class ThreadQueue
{
private:
	CRITICAL_SECTION cs_;
	queue<T> queue_;
	int queue_max_size_;
public:
	ThreadQueue()
	{
		InitializeCriticalSection(&cs_);
		queue_max_size_ = 1000;
	}
	~ThreadQueue()
	{
		DeleteCriticalSection(&cs_);
	}
	
	void set_max_size(int size)
	{
		queue_max_size_ = size;
	}

	void lock()
	{
		EnterCriticalSection(&cs_);
	}

	void unlock()
	{
		LeaveCriticalSection(&cs_);
	}

	bool is_empty(bool is_lock = true)
	{
		bool result;
		if (is_lock)
			lock();
		result = queue_.empty();
		if (is_lock)
			unlock();
		return result;
	}

	T pop(bool is_lock = true)
	{
		T temp;
		if (is_lock)
			lock();
		if (!queue_.empty())
		{
			temp = queue_.front();
			queue_.pop();
		}
		if (is_lock)
			unlock();
		return temp;
	}

	bool push(T object, bool is_lock = true)
	{
		if (is_lock)
			lock();
		if (queue_.size() >= queue_max_size_)
		{
			if (is_lock)
				unlock();
			return false;
		}
		queue_.push(object);
		if (is_lock)
			unlock();
		return true;
	}

	void clean_queue(bool is_lock = true)
	{
		if (is_lock)
			lock();
		while (!queue_.empty())
			queue_.pop();
		if (is_lock)
			unlock();
	}
};
