#pragma once

#include <asio/asio.hpp>
#include "net_message.h"
#include <deque>
#include <iostream>
#include "ts_queue.h"

#undef SendMessage // somehow scope is being polluted by windows header files

using connectionID = unsigned int;

class net_connection : public std::enable_shared_from_this<net_connection> {

public:
	net_connection(asio::io_context& context, TSQue<net_message>& msg_queue, connectionID id);
	virtual ~net_connection();

	void SendMessage(const net_message& message);

	void Connect(const std::string& ip, int port);

	void Disconnect();

	void Start();

	bool IsConnected();

	TSQue<net_message>& Recieved();

	asio::ip::tcp::socket& Socket();

	int ID() { return m_ID; }

private:

	void StartWriteHeader();
	void StartWriteBody();

	void StartReadHeader();
	void StartReadMessage();

private:

	net_message m_current_msg_in;
	TSQue<net_message>& m_msg_in;
	TSQue<net_message> m_msg_out;
	asio::io_context& m_asio_context;
	asio::ip::tcp::socket m_socket;

	connectionID m_ID = 0;
};