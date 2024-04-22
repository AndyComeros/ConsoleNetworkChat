#pragma once

#include <deque>
#include <iostream>
#include <asio/asio.hpp>
#include "net_message.h"
#include "net_connection.h"
#include "ts_queue.h"

class net_server {
public:
	net_server(int port);
	virtual ~net_server();

	void Start();
	void Stop();

	void StartAcceptConnection();

private:

	std::unique_ptr<std::thread> m_asio_thead;

	std::vector<std::shared_ptr<net_connection>> m_connections;
	std::shared_ptr<net_connection> m_newConnection;

	asio::io_context m_asio_context;
	asio::ip::tcp::acceptor m_acceptor;

	TSQue<net_message> m_message_in;

};
