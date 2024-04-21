#pragma once

#include <deque>
#include <iostream>
#include <asio/asio.hpp>
#include "net_message.h"
#include "net_connection.h"


class net_server {
public:
	net_server(int port);
	virtual ~net_server();

	void start();
	void stop();

	void start_accept_connection();

private:

	std::unique_ptr<std::thread> m_asio_thead;

	std::deque<std::shared_ptr<net_connection>> m_connections;
	std::shared_ptr<net_connection> m_newConnection;

	asio::io_context m_asio_context;
	std::deque<net_message> m_message_in;
	asio::ip::tcp::acceptor m_acceptor;
};
