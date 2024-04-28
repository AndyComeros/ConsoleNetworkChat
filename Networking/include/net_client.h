#pragma once

#include <asio/asio.hpp>
#include "net_message.h"
#include "net_connection.h"

class net_client { 
public:
	net_client();
	virtual ~net_client();

	void Start();

	void Connect(const std::string& ip, uint16_t port);

	void Disconnect();

	TSQue<net_message>& Messages();

	net_connection& Connection();

protected:
	virtual void OnConnect();

private:
	asio::io_context m_asio_context;
	net_connection m_connection;

	std::unique_ptr<std::thread> m_asio_thread;
	TSQue<net_message> m_messages;
};