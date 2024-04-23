#include "../net_connection.h"

net_connection::net_connection(asio::io_context& context, TSQue<net_message>& msg_queue) : m_asio_context(context),
m_socket(context),
m_msg_in(msg_queue)
{
	std::cout << "connection ctor" << std::endl;
}

net_connection::~net_connection()
{
	Disconnect();
}

void net_connection::SendMessage(net_message& message)
{
	// there is probably something wrong here...
	asio::post([&, message]() {

		// empty que means no messages are being written
		// if its not empty we can assume WriteHeader has already been called
		bool qEmpty = m_msg_out.Empty();
		m_msg_out.PushBack(message);
		
		if (qEmpty)
			WriteHeader();
	});
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
			std::cerr << "[net_connection]: " << ec.message() << std::endl;
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

void net_connection::WriteHeader()
{
	asio::async_write(m_socket, asio::buffer(&m_msg_out.Front().header, sizeof(net_message_header)),
		[&](const asio::error_code& ec, size_t bytes) {

			if (!ec) {
				WriteBody();
			}
			else {
				//m_msg_out.PopFront();
				std::cerr << "[Connection::WriteBody]: " << ec.message() << std::endl;
			}
		});
}

void net_connection::WriteBody()
{
	asio::async_write(m_socket, asio::buffer(m_msg_out.Front().contents.data(), m_msg_out.Front().contents.size()),
		[&](const asio::error_code& ec, size_t bytes) {

			if (!ec) {
				m_msg_out.PopFront();

				if (!m_msg_out.Empty()) {

					WriteHeader();
				}
			}
			else {
				std::cerr << "[Connection::WriteBody]: " << ec.message() << std::endl;
			}
		});
}


