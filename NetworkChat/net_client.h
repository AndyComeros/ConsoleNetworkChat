#pragma once

#include <deque>
#include <iostream>
#include <asio/asio.hpp>
#include "net_message.h"
#include "net_connection.h"


// at the moment will try to read sent messages and then print them to the screen.
// need to implement on message function
class net_client { 
public:

	net_client();
	virtual ~net_client();

	void start();

	void Connect(const std::string& ip, int port);
	void Disconnect();

	void StartReadHeader();
	void StartReadMessage();//not in use yet

private:

	//std::thread context_thread;
	asio::io_context m_asio_context;
	asio::ip::tcp::socket m_socket;
	std::deque<net_message> m_messages;
	std::vector<char> readbuff;//demo purposes, switch for proper queue later
};