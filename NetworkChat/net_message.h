#pragma once

#include <iostream>
#include <cstring>
#include <cstddef>

struct net_message_header {
	
};

class net_message {

	static const int MAX_DATA = 512;

public:
	

private:
	size_t m_size;
	char m_data[MAX_DATA];
};