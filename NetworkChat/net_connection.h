#pragma once

#include <asio/asio.hpp>
#include "net_message.h"
#include <deque>

class net_connection : public std::enable_shared_from_this<net_connection> {

public:
	net_connection(asio::io_context& context, std::deque<net_message>& msg_queue);
	virtual ~net_connection();

	void SendMessage(net_message& message);

	void Connect(const std::string& ip, int port);

	void Disconnect();

	bool IsConnected();

	asio::ip::tcp::socket& Socket();
private:

	std::deque<net_message>& msg_in;
	std::deque<net_message> msg_out;
	asio::io_context& m_asio_context;
	asio::ip::tcp::socket m_socket;
};