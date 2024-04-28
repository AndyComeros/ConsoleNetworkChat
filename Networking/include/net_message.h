#pragma once

#include <vector>
#include <cstring>

struct net_message_header {

	int type = 0;
	uint32_t data_size = 0;
};

struct net_message {
	
	net_message_header header;
	std::vector<char> payload;

	template<typename T>
	void AddData(const T& nData);
	void AddData(const void* nData, size_t size);
	size_t TotalSize();
};


template<typename T>
inline void net_message::AddData(const T& nData)
{
	size_t s = payload.size();

	payload.resize(payload.size() + sizeof(nData));
	std::memcpy(payload.data() + s, &nData, sizeof(T));

	header.data_size = payload.size();
}

inline void net_message::AddData(const void* nData, size_t size)
{
	size_t s = payload.size();

	payload.resize(payload.size() + size);
	std::memcpy(payload.data() + s, nData, size);

	header.data_size = payload.size();
}

inline size_t net_message::TotalSize()
{
	return sizeof(net_message_header) + payload.size();
}
