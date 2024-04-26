#pragma once

#include <deque>
#include <mutex>


// Simple thread safe std::dequeue wrapper class.
template<typename T>
class TSQue {
public:

	TSQue() = default;
	virtual ~TSQue() { Clear(); }

	void PushFront(const T& val);
	void PushBack(const T& val);

	const T& Front();
	const T& Back();

	T PopFront();
	T PopBack();

	void Clear();

	bool Empty();

private:

	std::deque<T> m_queue;
	std::mutex m_mutex;

};

template<typename T>
inline void TSQue<T>::PushFront(const T& val)
{
	std::scoped_lock(m_mutex);
	m_queue.push_front(val);
}

template<typename T>
inline void TSQue<T>::PushBack(const T& val)
{
	std::scoped_lock(m_mutex);
	m_queue.push_back(val);
}

template<typename T>
inline const T& TSQue<T>::Front()
{
	std::scoped_lock(m_mutex);
	return m_queue.front();
}

template<typename T>
inline const T& TSQue<T>::Back()
{
	std::scoped_lock(m_mutex);
	return m_queue.back();
}

template<typename T>
inline T TSQue<T>::PopFront()
{
	std::scoped_lock(m_mutex);
	T tempVal = std::move(m_queue.front());// apparently better that creating a new temp val 
	m_queue.pop_front();
	return tempVal;
}

template<typename T>
inline T TSQue<T>::PopBack()
{
	std::scoped_lock(m_mutex);
	T tempVal = std::move(m_queue.back());// apparently better that creating a new temp val 
	m_queue.pop_back();
	return tempVal;
}

template<typename T>
inline void TSQue<T>::Clear()
{
	std::scoped_lock(m_mutex);
	m_queue.clear();
}

template<typename T>
inline bool TSQue<T>::Empty()
{
	std::scoped_lock(m_mutex);
	return m_queue.empty();
}
