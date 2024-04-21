#pragma once

#include <vector>

struct net_message_header {

	int type = 0;
	size_t data_size = 0;
};


struct net_message {
	
	net_message_header header;
	std::vector<char> contents;

	template<typename T>
	void AddData(T nData);
};


template<typename T>
inline void net_message::AddData(T nData)
{
	size_t s = contents.size();

	contents.resize(contents.size() + sizeof(nData));
	std::memcpy(contents.data() + s, &nData, sizeof(T));

	header.data_size = contents.size();
}
