#pragma once

#include <asio/asio.hpp>
#include "net_message.h"
#include <deque>
#include <iostream>
#include "ts_queue.h"
#undef SendMessage // somehow scope is being polluted by windows header files

class net_connection : public std::enable_shared_from_this<net_connection> {

public:
	net_connection(asio::io_context& context, TSQue<net_message>& msg_queue);
	virtual ~net_connection();

	void SendMessage(net_message& message);

	void Connect(const std::string& ip, int port);

	void Disconnect();

	bool IsConnected();

	asio::ip::tcp::socket& Socket();

private:

	void WriteHeader();
	void WriteBody();

private:

	TSQue<net_message>& m_msg_in;
	TSQue<net_message> m_msg_out;
	asio::io_context& m_asio_context;
	asio::ip::tcp::socket m_socket;
};