#pragma once

#include <vector>

struct net_message_header {

	int type = 0;
	uint32_t data_size = 0;
};


struct net_message {
	
	net_message_header header;
	std::vector<char> contents;

	template<typename T>
	void AddData(const T& nData);
	void AddData(const void* nData, size_t size);
	size_t TotalSize();
};


template<typename T>
inline void net_message::AddData(const T& nData)
{
	size_t s = contents.size();

	contents.resize(contents.size() + sizeof(nData));
	std::memcpy(contents.data() + s, &nData, sizeof(T));

	header.data_size = contents.size();
}

inline void net_message::AddData(const void* nData, size_t size)
{
	size_t s = contents.size();

	contents.resize(contents.size() + size);
	std::memcpy(contents.data() + s, nData, size);

	header.data_size = contents.size();
}

inline size_t net_message::TotalSize()
{
	return sizeof(net_message_header) + (contents.size() * (sizeof(char)));
}
