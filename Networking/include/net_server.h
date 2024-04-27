#pragma once

#include <deque>
#include <iostream>
#include <asio/asio.hpp>
#include "net_message.h"
#include "net_connection.h"
#include "ts_queue.h"
#include "unordered_map"

using connection_map = std::unordered_map<connectionID, std::shared_ptr<net_connection>>;

class net_server {
public:
	net_server(int port);
	virtual ~net_server();

	void Start();
	void Stop();

	void StartAcceptConnection();

	void SendMessage(const net_message& msg, connectionID id);

	void BroadcastMessage(const net_message& msg);

	TSQue<net_message>& Messages();

	connection_map& Connections();
private:

	std::unique_ptr<std::thread> m_asio_thread;
	asio::io_context m_asio_context;
	asio::ip::tcp::acceptor m_acceptor;

	connection_map m_connections;
	std::shared_ptr<net_connection> m_newConnection;
	TSQue<net_message> m_messages;// recieved message queue to be read in.

	connectionID m_nextID = 0;
};
