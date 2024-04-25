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

	void Start();

	void Connect(const std::string& ip, int port);
	void Disconnect();

private:
	
	// should probs be in the connection class along with StartReadHeader and StartReadMessage?
	net_message m_current_message;// message currently being read in;


	asio::io_context m_asio_context;
	net_connection m_connection;

	std::unique_ptr<std::thread> m_asio_thread;
	TSQue<net_message> m_messages;
};