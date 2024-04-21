#include "../net_connection.h"

net_connection::net_connection(asio::io_context& context, std::deque<net_message>& msg_queue) : m_asio_context(context),
m_socket(context),
msg_in(msg_queue)
{
	std::cout << "connection ctor" << std::endl;
}

net_connection::~net_connection()
{
	Disconnect();
}

void net_connection::SendMessage(net_message& message)
{
	
}

void net_connection::Connect(const std::string& ip, int port)
{
	asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(ip), port);
	m_socket.async_connect(endpoint, [&](const asio::error_code& ec) {
		
		if (!ec)
		{
			std::cout << "connected!" << std::endl;
		}
		else {
			std::cout << "[net_connection]: " << ec.message() << std::endl;
		}
	});
}

void net_connection::Disconnect()
{
	if (IsConnected())
		asio::post(m_asio_context, [this]() { m_socket.close(); });
}

bool net_connection::IsConnected()
{
	return m_socket.is_open();
}

asio::ip::tcp::socket& net_connection::Socket()
{
	return m_socket;
}


